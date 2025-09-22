#include "Solver.h"
#include <cmath>
#include <iostream>

CGSolver::CGSolver(const SparseMatrix &_A, double _tol, int _maxIters)
    : A(_A), tol(_tol), maxIters(_maxIters)
{}

CGSolver::~CGSolver() {}

bool CGSolver::solve(const std::vector<double> &b, std::vector<double> &x) {
    int N = A.N;
    x.assign(N, 0.0);  // start from zero
    std::vector<double> r(N), p(N), Ap(N);

    // r = b - A*x = b
    for(int i = 0; i < N; ++i) {
        r[i] = b[i];
    }
    p = r;
    double rsold = 0.0;
    for(int i = 0; i < N; ++i) rsold += r[i]*r[i];
    double bnorm2 = rsold;
    if(bnorm2 < 1e-30) {
        // b is (near) zero ⇒ trivial solution x=0
        return true;
    }

    for(int iter = 0; iter < maxIters; ++iter) {
        A.matVec(p, Ap);
        double alpha_denom = 0.0;
        for(int i = 0; i < N; ++i) alpha_denom += p[i] * Ap[i];
        if(std::fabs(alpha_denom) < 1e-30) break;

        double alpha = rsold / alpha_denom;
        for(int i = 0; i < N; ++i) {
            x[i] += alpha * p[i];
            r[i] -= alpha * Ap[i];
        }
        double rsnew = 0.0;
        for(int i = 0; i < N; ++i) rsnew += r[i]*r[i];
        if(std::sqrt(rsnew / bnorm2) < tol) {
            return true;
        }
        double beta = rsnew / rsold;
        for(int i = 0; i < N; ++i) {
            p[i] = r[i] + beta * p[i];
        }
        rsold = rsnew;
    }
    // not converged within maxIters
    std::cerr << "Warning: CG solver did not converge in " << maxIters << " iters;\n"
              << "         final rel-res = " << std::sqrt(rsold/bnorm2) << "\n";
    return false;
}
