#ifndef UTILS_H
#define UTILS_H

#include "Mesh.h"
#include "SparseMatrix.h"

// -- write k²(x,y) at each mesh vertex to "ksq.txt"
void computeAndWriteKsq(double delta,
                        const MeshData &mesh,
                        const std::string &outfile = "ksq.txt");

// -- assemble A, M using CoLSaMM
void assembleMatricesColSaMM(const MeshData &mesh,
                      double delta,
                      SparseMatrix &outA,
                      SparseMatrix &outM);

// -- assemble A, M using your own 3‐point quadrature
void assembleWithQuadrature(const MeshData &mesh,
                            double delta,
                            SparseMatrix &outA,
                            SparseMatrix &outM);



// -- write eigenmode (x_i, y_i, u_i) to file
void writeEigenmode(const MeshData &mesh,
                    const std::vector<double> &u,
                    const std::string &outfile = "eigenmode.txt");

#endif // UTILS_H
