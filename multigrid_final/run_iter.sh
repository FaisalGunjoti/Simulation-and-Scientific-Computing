#!/usr/bin/env bash
# ------------------------------------------------------------
#  run_iter.sh     – generate results.dat for l = 3 … 13
#
#  usage:  ./run_iter.sh [--fmg] [maxLevel] [nIter]
#          --fmg      -> replace first cycle by FMG
#          maxLevel   -> highest l  (default 13)
#          nIter      -> how many table lines to capture (default 10)
#
#  output: results.dat   columns  lev  n  iter  err  relres
# ------------------------------------------------------------

set -euo pipefail

# ------------------ parse command-line ----------------------
use_fmg=false
[[ $# -gt 0 && $1 == "--fmg" ]] && { use_fmg=true; shift; }

maxL=${1:-13}
iters=${2:-10}

flag=$([[ $use_fmg == true ]] && echo "--fmg" || echo "")
echo "Running mgsolve for l = 3 .. $maxL   ($iters iterations)  $flag"

# ------------------ run solver on each level ---------------
rm -f results.dat
for (( L=3; L<=maxL; ++L )); do
    N=$(( (1<<L) + 1 ))
    n=$(( (N-2)*(N-2) ))
    printf "  l=%2d  N=%5d  n=%9d ...\n" "$L" "$N" "$n"

    ./mgsolve "$L" "$iters" $flag > out.txt

    # iteration lines start with a number (column 1)
    # mgsolve prints:  iter  L2-error  rel.res.  q
    awk -v lev="$L" -v ndof="$n" '
        /^[[:space:]]*[0-9]+/ {
            printf "%d %d %d %s %s\n", lev, ndof, $1, $2, $3
        }' out.txt >> results.dat
done

echo "Done  → results.dat created with $(wc -l < results.dat) lines"
