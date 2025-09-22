#include "multigrid.h"
#include <cassert>
#include <algorithm>   // std::fill

/*------------------------------ constructor --------------------------------*/

Multigrid::Multigrid(int L) : levels_(L)
{
    assert(L >= 1);
    /* create finest first, so index 0 = finest grid */
    for (int lev = L; lev >= 1; --lev)
        grids_.emplace_back(lev);
}

/*----------------------------- helper kernels -----------------------------*/

/* full-weighting restriction: r_h → b_H  (fine → coarse)  */
void Multigrid::restrictFullWeighting(int lev_f)
{
    Grid &F = grids_[lev_f];       // fine grid (level lev_f)
    Grid &C = grids_[lev_f + 1];   // its parent coarse grid

    /* The coarse grid stores the *error equation* => zero everywhere */
    std::fill(C.u.begin(), C.u.end(), 0.0);   // coarse solution = 0
    std::fill(C.b.begin(), C.b.end(), 0.0);   // coarse rhs = 0 (will fill interior)

    int Nc = C.N;
    for (int ic = 1; ic < Nc - 1; ++ic)
        for (int jc = 1; jc < Nc - 1; ++jc) {
            int ir = 2 * ic, jr = 2 * jc;                 // map to fine grid

            double sum =
                4.0 * F.r[F.idx(ir, jr)] +
                2.0 * ( F.r[F.idx(ir - 1, jr)] + F.r[F.idx(ir + 1, jr)] +
                        F.r[F.idx(ir, jr - 1)] + F.r[F.idx(ir, jr + 1)] ) +
                ( F.r[F.idx(ir - 1, jr - 1)] + F.r[F.idx(ir - 1, jr + 1)] +
                  F.r[F.idx(ir + 1, jr - 1)] + F.r[F.idx(ir + 1, jr + 1)] );

            C.b[C.idx(ic, jc)] = sum / 16.0;
        }
}

/* bilinear prolongation: e_H → e_h  (coarse → fine, add to fine solution) */
void Multigrid::prolongateBilinear(int lev_c)
{
    Grid &C = grids_[lev_c];       // coarse grid (error just computed here)
    Grid &F = grids_[lev_c - 1];   // next finer grid to be corrected

    int Nf = F.N;
    for (int i = 1; i < Nf - 1; ++i)
        for (int j = 1; j < Nf - 1; ++j) {
            int ic = i / 2, jc = j / 2;

            double corr;
            if ((i & 1) == 0 && (j & 1) == 0)          /* (even, even)   */
                corr = C.u[C.idx(ic, jc)];
            else if ((i & 1) == 0)                     /* (even, odd)    */
                corr = 0.5 * ( C.u[C.idx(ic, jc)] + C.u[C.idx(ic, jc + 1)] );
            else if ((j & 1) == 0)                     /* (odd,  even)   */
                corr = 0.5 * ( C.u[C.idx(ic, jc)] + C.u[C.idx(ic + 1, jc)] );
            else                                        /* (odd,  odd)    */
                corr = 0.25 * ( C.u[C.idx(ic, jc)]     + C.u[C.idx(ic + 1, jc)] +
                                 C.u[C.idx(ic, jc + 1)] + C.u[C.idx(ic + 1, jc + 1)] );

            F.u[F.idx(i, j)] += corr;
        }
}

/* tiny exact solve on the coarsest grid */
void Multigrid::solveCoarsest(Grid &g)
{
    if (g.N == 3) {                         // only one interior point
        int k = g.idx(1, 1);
        double sum = g.u[g.idx(0, 1)] + g.u[g.idx(2, 1)] +
                     g.u[g.idx(1, 0)] + g.u[g.idx(1, 2)];
        g.u[k] = ( g.h * g.h * g.b[k] + sum ) / 4.0;
    } else {                                // safety if user picks very few levels
        g.gaussSeidel(10);
    }
}

/*-------------------------------- V-cycle ---------------------------------*/

void Multigrid::vcycle(int lev /* = 0 */)
{
    Grid &G = grids_[lev];                  // current grid level

    /* base case: coarsest level */
    if (lev == levels_ - 1) {
        solveCoarsest(G);
        return;
    }

    /* ---- pre-smoothing  (ν₁ = 2) ---- */
    G.gaussSeidel(2);

    /* ---- residual + restriction ---- */
    G.computeResidual();
    restrictFullWeighting(lev);

    /* ---- recurse on coarse grid ---- */
    vcycle(lev + 1);

    /* ---- prolongate error & correct ---- */
    prolongateBilinear(lev + 1);

    /* ---- post-smoothing (ν₂ = 1) ---- */
    G.gaussSeidel(1);
}
/*---------------- FMG loop -------------------------------------------*/

void Multigrid::solveWithFMG(int post_vcycles /* k */)
{
    /* ---------- step 1: exact solve on the coarsest grid ---------- */
    solveCoarsest(grids_.back());                     // levels_-1

    /* ---------- step 2: successively enlarge the solution ---------- */
    for (int lev = levels_ - 2; lev >= 0; --lev) {    // …,2,1,0
        prolongateBilinear(lev + 1);                  // inject e_H → u_h
        grids_[lev].gaussSeidel(1);                   // one smoothing sweep
    }

    /* ---------- step 3: optional extra V-cycles on the finest grid -- */
    for (int i = 0; i < post_vcycles; ++i)
        vcycle(0);
}