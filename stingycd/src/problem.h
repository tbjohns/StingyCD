#ifndef __PROBLEM_H_INCLUDED__
#define __PROBLEM_H_INCLUDED__ 

#include "common.h"
#include <vector>

namespace CDL1 {

  struct Problem {

    int n;
    int d;
    int nnz;
    double tolerance;
    bool verbose;
    double max_time;
    double lambda;
    bool sparsity_pattern_termination;
    std::vector<Column> A_cols; 
    std::vector<double> b;

    void prep_A(int n, int d);

    void add_nonzero(int row, int col, double value);

    void cache_A_constants();

    Problem() {
      tolerance = 1e-5;
      max_time = 3600;
      verbose = false;
      sparsity_pattern_termination = false;
    }
  
    void set_A_sparse(
      double* data, 
      unsigned int* indices, 
      unsigned int* indptr, 
      unsigned int nnz, 
      unsigned int n, 
      unsigned int d);

    void set_A_dense(
           double* data, int n, int d, long* strides);


    void set_b(double* data, int n);

    void set_tolerance(double value) {
      tolerance = value;
    }

    void set_verbose(bool value) {
      verbose = value;
    }

    void set_max_time(double value) {
      max_time = value;
    }

};

}

#endif 
