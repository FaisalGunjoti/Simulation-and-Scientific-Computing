#include "SparseMatrix.h"
#include <algorithm>
#include <fstream>
#include <iostream>

SparseMatrix::SparseMatrix(int _N)
    : N(_N), isFinalized(false)
{
    rowPtr.clear();
    colIdx.clear();
    values.clear();
    triplets.clear();
}

SparseMatrix::~SparseMatrix() {
    // nothing
}

void SparseMatrix::addEntry(int i, int j, double v) {
    if(isFinalized) {
        std::cerr << "Error: cannot addEntry after finalize().\n";
        std::exit(1);
    }
    triplets.emplace_back(i, j, v);
}

void SparseMatrix::finalize() {
    if(isFinalized) return;
    // 1) sort triplets by (i,j)
    std::sort(triplets.begin(), triplets.end(),
              [](auto &a, auto &b){
                  int ai = std::get<0>(a), aj = std::get<1>(a);
                  int bi = std::get<0>(b), bj = std::get<1>(b);
                  if(ai != bi) return ai < bi;
                  return aj < bj;
              });
    // 2) merge duplicates
    std::vector<std::tuple<int,int,double>> merged;
    merged.reserve(triplets.size());
    for(size_t k = 0; k < triplets.size(); ++k) {
        int i = std::get<0>(triplets[k]);
        int j = std::get<1>(triplets[k]);
        double v = std::get<2>(triplets[k]);
        if(!merged.empty()) {
            auto &last = merged.back();
            if(std::get<0>(last) == i && std::get<1>(last) == j) {
                // same slot; sum
                std::get<2>(last) += v;
                continue;
            }
        }
        merged.emplace_back(i,j,v);
    }
    triplets.swap(merged);

    // 3) build CSR
    rowPtr.assign(N+1, 0);
    colIdx.resize(triplets.size());
    values.resize(triplets.size());

    // count entries per row
    for(auto &t : triplets) {
        int i = std::get<0>(t);
        rowPtr[i+1]++;
    }
    // prefix-sum
    for(int i = 0; i < N; ++i) {
        rowPtr[i+1] += rowPtr[i];
    }
    // temp copy of rowPtr to use as “write pointer”
    std::vector<int> writePtr = rowPtr;
    // fill colIdx, values
    for(auto &t : triplets) {
        int i = std::get<0>(t);
        int j = std::get<1>(t);
        double v = std::get<2>(t);
        int idx = writePtr[i]++;
        colIdx[idx] = j;
        values[idx] = v;
    }
    // done
    isFinalized = true;
}

void SparseMatrix::matVec(const std::vector<double> &x, std::vector<double> &y) const {
    if(!isFinalized) {
        std::cerr << "Error: matVec called before finalize().\n";
        std::exit(1);
    }
    if((int)x.size() != N) {
        std::cerr << "Error: x.size()!=N in matVec.\n";
        std::exit(1);
    }
    y.assign(N, 0.0);
    for(int i = 0; i < N; ++i) {
        int rowStart = rowPtr[i], rowEnd = rowPtr[i+1];
        double sum = 0.0;
        for(int idx = rowStart; idx < rowEnd; ++idx) {
            sum += values[idx] * x[colIdx[idx]];
        }
        y[i] = sum;
    }
}

void SparseMatrix::writeToFile(const std::string &filename) const {
    if(!isFinalized) {
        std::cerr << "Error: writeToFile called before finalize().\n";
        std::exit(1);
    }
    std::ofstream out(filename);
    if(!out) {
        std::cerr << "Error: could not open \"" << filename << "\" for writing.\n";
        std::exit(1);
    }
    for(int i = 0; i < N; ++i) {
        for(int idx = rowPtr[i]; idx < rowPtr[i+1]; ++idx) {
            int j = colIdx[idx];
            double v = values[idx];
            if(v != 0.0) {
                out << i << " " << j << " " << v << "\n";
            }
        }
    }
    out.close();
}
