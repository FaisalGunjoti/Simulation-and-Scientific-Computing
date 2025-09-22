#include <iostream>
#include <iomanip>
#include <chrono>
#include <fstream>
#include <string>
#include "multigrid.h"

int main(int argc, char* argv[])
{
    /* ------------------------------------------------------------------ */
    /*  command-line:  mgsolve  <levels>  <cycles>  [--fmg]               */
    /* ------------------------------------------------------------------ */
    if (argc < 3 || argc > 4) {
        std::cerr << "Usage: " << argv[0]
                  << " <levels> <cycles> [--fmg]\n";
        return 1;
    }

    const bool use_fmg = (argc == 4 && std::string(argv[3]) == "--fmg");
    const int  L       = std::stoi(argv[1]);      // total grid levels
    const int  C       = std::stoi(argv[2]);      // lines to print (incl. 0)

    Multigrid mg(L);
    Grid      &G = mg.grid(0);                    // finest grid handle

    /* ---------------------------------------------------------- */
    /*  initial residual BEFORE any multigrid work                */
    /* ---------------------------------------------------------- */
    G.computeResidual();
    const double bnorm = G.normResidual();

    std::cout << "* Dirichlet Poisson 2-D, "
              << (use_fmg ? "FMG + V(2,1)" : "plain V(2,1)") << '\n'
              << "* Levels: " << L << ", finest N = " << G.N << '\n'
              << std::scientific << std::setprecision(3)
              << " iter   L2-error     rel.res.    q-factor\n"
              << "---------------------------------------------\n";

    /*  iteration-0 line (all zeros so far) */
    std::cout << std::setw(4) << 0 << "  "
              << std::setw(11) << G.computeError() << "  "
              << std::setw(11) << 1.0               << "   ---\n";

    /*  timers and previous residual for q-factor */
    const auto t0 = std::chrono::high_resolution_clock::now();
    double     rprev = bnorm;

    /* ================================================================== */
    /*  1)  FMG start-up branch                                           */
    /* ================================================================== */
    if (use_fmg) {
        /* ---------- FMG cascade (counts as iteration 1) ---------- */
        G.resetInterior();                 // clear fine grid unknowns
        mg.solveWithFMG(0);                         // private helper does the cascade

        G.computeResidual();
        double rn  = G.normResidual();
        double err = G.computeError();

        std::cout << std::setw(4) << 1 << "  "
                  << std::setw(11) << err     << "  "
                  << std::setw(11) << rn / bnorm << "  "
                  << std::setw(8)  << rn / rprev << '\n';

        rprev = rn;

        /* ---------- remaining V-cycles (iterations 2 … C) ---------- */
        for (int it = 2; it <= C; ++it) {
            mg.vcycle();
            G.computeResidual();

            rn  = G.normResidual();
            err = G.computeError();

            std::cout << std::setw(4) << it << "  "
                      << std::setw(11) << err        << "  "
                      << std::setw(11) << rn / bnorm << "  "
                      << std::setw(8)  << rn / rprev << '\n';

            rprev = rn;
        }
    }
    /* ================================================================== */
    /*  2)  “plain” multigrid branch                                      */
    /* ================================================================== */
    else {
        for (int it = 1; it <= C; ++it) {
            mg.vcycle();
            G.computeResidual();

            const double rn  = G.normResidual();
            const double err = G.computeError();

            std::cout << std::setw(4) << it << "  "
                      << std::setw(11) << err        << "  "
                      << std::setw(11) << rn / bnorm << "  "
                      << std::setw(8)  << rn / rprev << '\n';

            rprev = rn;
        }
    }

    /* ---------------------------------------------------------- */
    /*  timing + file output                                      */
    /* ---------------------------------------------------------- */
    const auto t1 = std::chrono::high_resolution_clock::now();
    const double sec = std::chrono::duration<double>(t1 - t0).count();

    std::cout << "* Total solve time: " << sec << " s\n";

    std::ofstream fout("solution.txt");
    for (int i = 0; i < G.N; ++i) {
        for (int j = 0; j < G.N; ++j)
            fout << i << ' ' << j << ' ' << G.u[G.idx(i, j)] << '\n';
        fout << '\n';
    }
    std::cout << "* Solution written to solution.txt\n";
    return 0;
}
