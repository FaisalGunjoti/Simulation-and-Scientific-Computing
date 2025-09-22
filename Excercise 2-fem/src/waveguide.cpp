#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <string>
#include <fstream>    
#include "Mesh.h"
#include "SparseMatrix.h"
#include "Solver.h"
#include "Utils.h"

constexpr double EIG_TOL = 1e-10;   // inverse‑iteration stopping tolerance

int main(int argc, char *argv[]) {
    
    bool useColsamm = false;
    if(argc == 4 && std::string(argv[3])=="--Colsamm") {
         useColsamm = true;
    } else if(argc != 3) {
         std::cerr << "Usage: " << argv[0] << " δ ε [--Colsamm]\n"
                   << "   --Colsamm: assemble via CoLSaMM (default is your bonus quadrature)\n";
         return 1;
     }
   // argv[1]=delta, argv[2]=tol; argv[3] if present is --Colsamm
    double delta = std::stod(argv[1]);
    double tol   = std::stod(argv[2]);

    //  Read mesh
    MeshData mesh;
    mesh.read("data/unit_circle.txt");
    int N = mesh.nVertices();
    std::cout << "Mesh: " << N << " vertices, "
              << mesh.nTriangles() << " triangles\n";

    // Compute and write k² at each vertex: ksq.txt
    computeAndWriteKsq(delta, mesh, "ksq.txt");
    std::cout << "  wrote ksq.txt\n";

    //  Assemble stiffness A_h and mass M_h via CoLSaMM
    SparseMatrix A, M;
    std::cout << "Assembling global matrices (this may take a moment)...\n";
    if (useColsamm) {
       // assembling the ColSaMM version 
       assembleMatricesColSaMM(mesh, delta, A, M);
   } else {
       // assembling the bonus quadrature version
       assembleWithQuadrature(mesh, delta, A, M);
   }
    std::cout << "  Assembled A_h and M_h (size " << N << "×" << N << ")\n";

    //  Write A.txt, M.txt
    A.writeToFile("A.txt");
    M.writeToFile("M.txt");
    std::cout << "  wrote A.txt, M.txt\n";

    //  Inverse‐power iteration to find smallest eigenvalue of A u = λ M u
    std::vector<double> u(N, 1.0), w(N, 0.0);
    // normalize u with respect to M‐weighted norm
    {
        double normM = 0.0;
        std::vector<double> Mu(N);
        M.matVec(u, Mu);
        for(int i = 0; i < N; ++i) normM += u[i] * Mu[i];
        normM = std::sqrt(normM);
        for(int i = 0; i < N; ++i) u[i] /= normM;
        
    }

    // --- Open a file to record λ at each iteration ---
    std::ofstream lambdaFile("lambda_iter.txt");
    if(!lambdaFile) {
        std::cerr << "Error: could not open lambda_iter.txt for writing\n";
        std::exit(1);
    }

    double lambda_old = 0.0;
    double lambda     = 0.0;
    int iter = 0;
    const int maxIter = 1000;

    std::cout << std::fixed << std::setprecision(12);
    for(iter = 1; iter <= maxIter; ++iter) {
        //  compute f = M * u   (rhs for A * w = f)
        std::vector<double> f(N);
        M.matVec(u, f);

        //  solve A w = f via CG
        CGSolver cg(A, tol, 5*N);  // maxIters = 5*N just in case
        bool ok = cg.solve(f, w);
        if(!ok) {
            std::cerr << "Warning: CG did not fully converge at iteration " << iter << "\n";
        }

        //  normalize w w.r.t. M‐norm → new u
        
        std::vector<double> Mw(N);
        M.matVec(w, Mw);
        double normM = 0.0;
        for(int i = 0; i < N; ++i) normM += w[i]*Mw[i];
        normM = std::sqrt(normM);
        if(normM < 1e-16) {
            std::cerr << "Error: CG returned nearly zero w; cannot normalize.\n";
            std::exit(1);
        }
        for(int i = 0; i < N; ++i) w[i] /= normM;
        M.matVec(w, Mw);
        //  Rayleigh quotient: λ = (wᵀ A w)/(wᵀ M w)
        
        std::vector<double> Aw(N);
        double num = 0.0, den = 0;
        A.matVec(w, Aw);
        for(int i = 0; i < N; ++i) {
            num += w[i] * Aw[i];
            den += w[i] * Mw[i];  // wᵀ M w = 1 after normalization
        }
        lambda = num/den;

        // --- Write the current iteration’s λ into the file ---
        lambdaFile << iter << " " << lambda << "\n";

        //  print λ and check convergence
        if(iter == 1) {
            std::cout << "Iter " << iter << ": λ = " << lambda << "\n";
        } else {
            double relChange = std::fabs(lambda - lambda_old) / std::fabs(lambda);
            std::cout << "Iter " << iter << ": λ = " << lambda
                      << "   (rel‐change = " << relChange << ")\n";
            if(relChange < EIG_TOL) {
                std::cout << "Converged (|λ_k - λ_{k−1}|/|λ_k| < " <<  EIG_TOL << ").\n";
                break;
            }
        }
        lambda_old = lambda;
        u = w;  // set u ← w for next iter
    }
    if(iter > maxIter) {
        std::cout << "Warning: inverse‐power did not converge in " << maxIter << " iterations.\n";
    }

    // Close the λ‐file
  lambdaFile.close();


//  Write eigenmode.txt
writeEigenmode(mesh, w, "eigenmode.txt");
std::cout << "Wrote eigenmode.txt (eigenvalue ≈ " << lambda << ")\n";
    return 0;
}
