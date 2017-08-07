#include "problem.h"
#include "common.h"
#include "logger.h"
#include <vector>
#include <cmath>
#include <iostream>

using std::vector;
using std::cout;
using std::endl;
using std::abs;

namespace CDL1 {

  void Problem::prep_A(int n, int d) {
    this->n = n;
    this->d = d;

    A_cols.clear();
    A_cols.resize(d);
  }

  void Problem::cache_A_constants() {
    for (int j = 0; j < d; j++) 
      A_cols[j].cache_constants(n);

    nnz = 0;
    for (int j = 0; j < d; j++)
      nnz += A_cols[j].indices.size();
  }

  void Problem::add_nonzero(int row, int col, double value) {
    if (value == 0)
      return;
    A_cols[col].append(row, value);
  }


  void Problem::set_A_sparse(double* data, unsigned int* indices, unsigned int* indptr, unsigned int nnz, unsigned int n, unsigned int d) {

    prep_A((int) n, (int) d);

    unsigned int i = 0;
    unsigned int col = 0;
    unsigned int col_end_i = indptr[1];
    while (i < nnz) {
      unsigned int row = indices[i];
      if (i == col_end_i) {
        col++;
        col_end_i = indptr[col+1];
      }
      add_nonzero((int) row, (int) col, data[i++]); 
    }

    cache_A_constants();
  }
    
  void Problem::set_A_dense(double* data, int n, int d, long* strides) {

    this->nnz = n*d;

    bool Column_major = (strides[0] < strides[1]);

    prep_A(n, d);

    int i = 0;
    if (Column_major) {
      for (int col=0; col < d; col++) {
        for (int row=0; row < n; row++)
          add_nonzero(row, col, data[i++]);
      }
    } else {
      for (int row=0; row < n; row++) {
        for (int col=0; col < d; col++)
          add_nonzero(row, col, data[i++]);
      }
    }

    cache_A_constants();
  }

  void Problem::set_b(double* data, int n) {
    b.resize(n);
    for (int i = 0; i < n; i++) {
      b[i] = data[i];
    }
  }

}
