#ifndef SPARSEMATRIX_H
#define SPARSEMATRIX_H

#include <vector>
#include <tuple>
#include <string>

// A simple CSR (compressed‐row) sparse‐matrix class for double‐precision.
//
// We first collect entries in a triplet list (i,j,value), then call finalize(),
// which builds the CSR structure.  We support:
//   - addEntry(i,j,val)  // can add multiple to same (i,j); they're summed
//   - matVec(x, y)       // y = A * x
//   - writeToFile(fn)    // writes nonzero entries (i j A[i,j]) row‐wise
//
// Rows/cols are 0-based.  The dimension is NxN, where N is specified at construction.

class SparseMatrix {
public:
    int N;                             // matrix dimension (NxN)
    std::vector<int> rowPtr;           // size = N+1
    std::vector<int> colIdx;           // size = nnz
    std::vector<double> values;        // size = nnz

    SparseMatrix(int _N=0);
    ~SparseMatrix();

    // temporarily store triplets until finalize() is called
    void addEntry(int i, int j, double v);

    // Build CSR from triplets (collect duplicates, sum them, sort by (i,j)).
    void finalize();

    // Multiply: y = this * x.  x,y length N.
    void matVec(const std::vector<double> &x, std::vector<double> &y) const;

    // Write nonzero pattern to file: each line “i j A[i,j]”
    void writeToFile(const std::string &filename) const;

private:
    std::vector<std::tuple<int,int,double>> triplets;
    bool isFinalized;
};

#endif // SPARSEMATRIX_H
