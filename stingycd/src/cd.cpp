#include "solver.h"
#include "problem.h"

using CDL1::Solver;
using CDL1::Problem;

extern "C" {
  Problem* CD_new() { 
    Problem* bp = new Problem();
    return bp; 
  }

  void CD_set_A_dense(
      Problem* bp, 
      double* data, 
      int n, 
      int d, 
      long* strides) {
    bp->set_A_dense(data, n, d, strides);
  }

  void CD_set_A_sparse(
      Problem* bp, 
      double* data, 
      unsigned int* indices, 
      unsigned int* indptr, 
      unsigned int nnz, 
      unsigned int n, 
      unsigned int d) {
    bp->set_A_sparse(data, indices, indptr, nnz, n, d);
  }

  void CD_set_b(Problem* bp, double* data, int length) {  
    bp->set_b(data, length);
  }

  void CD_set_tolerance(Problem* bp, double value) {
    bp->set_tolerance(value);
  }

  void CD_set_verbose(Problem* bp, bool value) {
    bp->set_verbose(value);
  }

  void CD_set_max_time(Problem* bp, double value) {
    bp->set_max_time(value);
  }

  void CD_delete(Problem* bp) {
    delete bp;
  }

  void CD_solve(
      Problem* bp, 
      double lambda, 
      double* result, 
      double* residuals, 
      double* obj,
      const char* log_dir) {
    Solver solver(*bp);
    solver.solve(
      lambda, result, residuals, obj, log_dir);
  }
  
}
