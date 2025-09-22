#ifndef MULTIGRID_H
#define MULTIGRID_H

#include <vector>
#include "grid.h"

class Multigrid {
public:
    explicit Multigrid(int levels);

    // one V(2,1) cycle starting at level 0 (finest)
    void vcycle(int lev = 0);

    // FMG start-up +k ordinary cycles
     void solveWithFMG(int k_vcycles);

    Grid&       grid(int lev)       { return grids_.at(lev); }
    const Grid& grid(int lev) const { return grids_.at(lev); }
private:
    int levels_;                 // total number of levels
    std::vector<Grid> grids_;    // 0 = finest … levels_-1 = coarsest

    /* helpers */
    void restrictFullWeighting(int lev_f);            // fine → coarse rhs
    void prolongateBilinear(int lev_c);               // coarse error → fine
    static void solveCoarsest(Grid &g);

                                   
};

#endif
