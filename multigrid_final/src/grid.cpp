#include "grid.h"
#include <cmath>

namespace {
    inline double f(double x, double y) {      // right-hand side
        return 2.0 * M_PI * M_PI * std::cos(M_PI * x) * std::cos(M_PI * y);
    }
    inline double g(double x, double y) {      // boundary & exact solution
        return std::cos(M_PI * x) * std::cos(M_PI * y);
    }
} // namespace

Grid::Grid(int level)
{
    N  = (1 << level) + 1;          // 2^level + 1 grid points
    h  = 1.0 / (N - 1);
    inv_h2 = 1.0 / (h * h);

    u.assign(N * N, 0.0);
    b.assign(N * N, 0.0);
    r.assign(N * N, 0.0);

    initBoundary();
    initRHS();
}

/*--------------------------------- initialisation -------------------------*/

void Grid::initBoundary()
{
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            if (i == 0 || j == 0 || i == N - 1 || j == N - 1)
                u[idx(i, j)] = g(i * h, j * h);
}

void Grid::initRHS()
{
    for (int i = 1; i < N - 1; ++i)
        for (int j = 1; j < N - 1; ++j)
            b[idx(i, j)] = f(i * h, j * h);
}

/*---------------------------------- kernels -------------------------------*/

void Grid::resetInterior()
{
    for (int i = 1; i < N - 1; ++i)
        for (int j = 1; j < N - 1; ++j)
            u[idx(i, j)] = 0.0;
}

void Grid::computeResidual()
{
    for (int i = 1; i < N - 1; ++i)
        for (int j = 1; j < N - 1; ++j) {
            int k = idx(i, j);
            double Au = ( 4.0 * u[k]
                         - u[idx(i - 1, j)] - u[idx(i + 1, j)]
                         - u[idx(i, j - 1)] - u[idx(i, j + 1)] ) * inv_h2;
            r[k] = b[k] - Au;
        }
}

void Grid::gaussSeidel(int sweeps)
{
    for (int sw = 0; sw < sweeps; ++sw)
        for (int i = 1; i < N - 1; ++i)
            for (int j = 1; j < N - 1; ++j) {
                int k = idx(i, j);
                u[k] = 0.25 * ( u[idx(i - 1, j)] + u[idx(i + 1, j)]
                              + u[idx(i, j - 1)] + u[idx(i, j + 1)]
                              + h * h * b[k] );
            }
}

/*--------------------------------- diagnostics ----------------------------*/

double Grid::normResidual() const
{
    double sum = 0.0;
    for (double v : r) sum += v * v;
    return std::sqrt(sum);
}

double Grid::computeError() const
{
    double sum = 0.0;
    for (int i = 1; i < N - 1; ++i)
        for (int j = 1; j < N - 1; ++j) {
            double diff = u[idx(i, j)] - g(i * h, j * h);
            sum += diff * diff;
        }
    int n = (N - 2) * (N - 2);
    return std::sqrt(sum / n);      // L² norm divided by √n (for plotting)
}
