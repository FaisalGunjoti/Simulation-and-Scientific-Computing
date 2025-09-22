#ifndef GRID_H
#define GRID_H

#include <vector>

class Grid {
public:
    int    N;           // total points per dimension  (incl. boundary)
    double h;           // mesh width
    double inv_h2;      // 1 / h²  (cached)

    std::vector<double> u, b, r;

    explicit Grid(int level);

    inline int idx(int i, int j) const { return i * N + j; }

    void initBoundary();
    void initRHS();

    void resetInterior();                 // zero unknowns (keeps boundary)
    void computeResidual();
    void gaussSeidel(int sweeps = 1);

    double normResidual() const;
    double computeError() const;          // discrete L² (scaled by 1/√n)
};

#endif
