// src/Utils.cpp

#include "Utils.h"
#include "Colsamm.h"   // must exist under include/
#include <fstream>
#include <cmath>
#include <iostream>
#include <algorithm>  // for std::max

using namespace _COLSAMM_;           // CoLSaMM namespace
using IA = IntegrationAccuracy;     // shorthand for Gauss2, etc.

namespace {
    double g_delta = 0.0;  // for k2_func when doing CoLSaMM func-integral

    // helper for the CoLSaMM k²·φ_j·φ_i integral
    double k2_func(double x, double y) {
        return (100.0 + g_delta) * std::exp(-50.0*(x*x + y*y)) - 100.0;
    }
}

// ----------------------------------------------------------------------------
//  Write k² at each mesh vertex
void computeAndWriteKsq(double delta,
                        const MeshData &mesh,
                        const std::string &outfile)
{
    std::ofstream out(outfile);
    if(!out) {
        std::cerr << "Error: cannot open \""<<outfile<<"\" for writing ksq.\n";
        std::exit(1);
    }

    int N = mesh.nVertices();
    for(int i=0; i<N; ++i){
        double x = mesh.x[i], y = mesh.y[i];
        double k2 = (100.0+delta)*std::exp(-50.0*(x*x+y*y)) - 100.0;
        out << x << " " << y << " " << k2 << "\n";
    }
    out.close();
}

// ----------------------------------------------------------------------------
//  Assemble via CoLSaMM
void assembleMatricesColSaMM(const MeshData &mesh,
                      double delta,
                      SparseMatrix &outA,
                      SparseMatrix &outM)
{
    // stash delta for the func<> call
    g_delta = delta;

    int N = mesh.nVertices();
    outA = SparseMatrix(N);
    outM = SparseMatrix(N);

    // precompute k² at vertices (for the un-weighted mass term if needed)
    std::vector<double> k2atV(N);
    for(int i=0; i<N; ++i){
        double x=mesh.x[i], y=mesh.y[i];
        k2atV[i] = (100.0+delta)*std::exp(-50.0*(x*x+y*y)) - 100.0;
    }

    // choose 3-point Gauss2 rule on reference triangle
    using Tri = ELEMENTS::_Triangle_<IA::Gauss2>;
    Tri tri;

    // loop over mesh triangles
    for(auto &T : mesh.triangles){
        int i0=T[0], i1=T[1], i2=T[2];
        std::array<double,6> coords = {
            mesh.x[i0], mesh.y[i0],
            mesh.x[i1], mesh.y[i1],
            mesh.x[i2], mesh.y[i2]
        };
        tri(coords);

        // local stiffness ∫ ∇φ_j·∇φ_i
        auto localStiff = tri.integrate( grad(v_()) * grad(w_()) );

        // local mass ∫ φ_j φ_i
        auto localMass  = tri.integrate( v_() * w_() );

        // k²-weighted mass ∫ k² φ_j φ_i
        auto localK2    = tri.integrate( func<double>(k2_func) * v_() * w_() );

        // scatter into global
        for(int a=0; a<3; ++a) {
            for(int b=0; b<3; ++b) {
                double Aab = localStiff[a][b] - localK2[a][b];
                double Mab = localMass[a][b];
                int row = T[a], col = T[b];
                if(std::fabs(Aab) > 1e-16) outA.addEntry(row,col,Aab);
                if(std::fabs(Mab) > 1e-16) outM.addEntry(row,col,Mab);
            }
        }
    }

    outA.finalize();
    outM.finalize();
}

// ----------------------------------------------------------------------------
//  Bonus: assemble via own 3-point quadrature on each triangle
void assembleWithQuadrature(const MeshData &mesh,
                            double delta,
                            SparseMatrix &outA,
                            SparseMatrix &outM)
{
    int N = mesh.nVertices();
    outA = SparseMatrix(N);
    outM = SparseMatrix(N);

    // quadrature on reference triangle conv{(0,0),(1,0),(0,1)}
    constexpr double wq[3]   = {1.0/6, 1.0/6, 1.0/6};
    constexpr double qx[3] = {1.0/6, 2.0/3, 1.0/6};
    constexpr double qy[3] = {1.0/6, 1.0/6, 2.0/3};

    // precompute k2 at vertices
    std::vector<double> k2atV(N);
    for(int i=0; i<N; ++i){
        double x=mesh.x[i], y=mesh.y[i];
        k2atV[i] = (100.0+delta)*std::exp(-50.0*(x*x+y*y)) - 100.0;
    }

    // loop triangles
    for(auto &T : mesh.triangles){
        int i0=T[0], i1=T[1], i2=T[2];
        double x0=mesh.x[i0], y0=mesh.y[i0];
        double x1=mesh.x[i1], y1=mesh.y[i1];
        double x2=mesh.x[i2], y2=mesh.y[i2];

        // Jacobian and area
        double J11=x1-x0, J12=x2-x0;
        double J21=y1-y0, J22=y2-y0;
        double detJ = J11*J22 - J12*J21;
        double area = std::fabs(detJ)*0.5;

        // constant ∇φ on this triangle
        double inv2A = 1.0/(2.0*area);
        double gx[3] = { (y1-y2)*inv2A,
                         (y2-y0)*inv2A,
                         (y0-y1)*inv2A };
        double gy[3] = {-(x1-x2)*inv2A,
                        -(x2-x0)*inv2A,
                        -(x0-x1)*inv2A };

        // average k² over the three vertices
        double kavg = (k2atV[i0]+k2atV[i1]+k2atV[i2]) / 3.0;

        // local element matrices
        double A_loc[3][3] = {{0}}, M_loc[3][3] = {{0}};

        // 3-point quadrature
        for(int qp=0; qp<3; ++qp){
            double xi=qx[qp], eta=qy[qp];
            double phi[3] = {1 - xi - eta, xi, eta};

            for(int a=0;a<3;++a){
                for(int b=0;b<3;++b){
                    double stiff = gx[a]*gx[b] + gy[a]*gy[b];
                    double mass  = phi[a]*phi[b];
                    A_loc[a][b] += wq[qp] * (stiff - kavg*mass) * detJ;
                    M_loc[a][b] += wq[qp] * mass * detJ;
                }
            }
        }

        // scatter
        int idx[3]={i0,i1,i2};
        for(int a=0;a<3;++a){
            for(int b=0;b<3;++b){
                if(std::fabs(A_loc[a][b])>1e-16)
                    outA.addEntry(idx[a], idx[b], A_loc[a][b]);
                if(std::fabs(M_loc[a][b])>1e-16)
                    outM.addEntry(idx[a], idx[b], M_loc[a][b]);
            }
        }
    }

    outA.finalize();
    outM.finalize();
}

// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// 4) Write eigenmode to file
void writeEigenmode(const MeshData &mesh,
                    const std::vector<double> &u,
                    const std::string &outfile)
{
    std::ofstream out(outfile);
    if(!out) {
        std::cerr<<"Error: cannot open "<<outfile<<" for eigenmode\n";
        std::exit(1);
    }
    int N = mesh.nVertices();
    for(int i=0;i<N;++i){
        out<<mesh.x[i]<<" "<<mesh.y[i]<<" "<<u[i]<<"\n";
    }
    out.close();
}
