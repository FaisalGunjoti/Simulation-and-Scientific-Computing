#ifndef SOLVER_H
#define SOLVER_H

#include <vector>
#include "SparseMatrix.h"

// Conjugate‐Gradient solver for SPD matrices.
// Solves A x = b for x, given A (CSR), b.  Returns true if converged within tol in maxIters.
//
// Usage:
//   CGSolver cg(A, tol, maxIters);
//   bool ok = cg.solve(b, x);
//   // x is filled.  Returns false if not converged within maxIters.

class CGSolver {
public:
    const SparseMatrix &A;
    double tol;
    int maxIters;

    CGSolver(const SparseMatrix &_A, double _tol, int _maxIters=10000);
    ~CGSolver();

    // Solve A x = b.  If initial guess is nonzero in x, it is used; else x is zeroed.
    // x must be of length N (A.N).  b must be same length.
    // Returns true if ||residual||/||b|| < tol within maxIters.
    bool solve(const std::vector<double> &b, std::vector<double> &x);
};

#endif // SOLVER_H
