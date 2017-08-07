#include "solver.h"
#include "timer.h"

#include <cstring>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <iostream>

using std::vector;
using std::cout;
using std::cerr;
using std::endl;
using std::abs;
using std::max;
using std::min;

namespace CDL1 {

Solver::Solver(Problem &problem) : p(problem) { }

double Solver::compute_primal_obj(double lambda) {
  return 0.5 * l2_sq(residuals) + lambda * l1(x);
}

void Solver::solve(
    double lambda, 
    double* x_result, 
    double* residuals_result, 
    double *obj_result,
    const char* log_dir) {

  // Timing/logging:
  cout.precision(15);
  timer.reset();
  Logger logger;
  logger.set_log_directory(log_dir);
  logger.throttle_logging_with_interval(p.max_time / 50);

  // Problem is n examples by d features:
  int d = p.A_cols.size();
  int n = p.b.size();

  // Initialize weight vector:
  x.assign(d, 0);

  // Initialize residuals vector:
  residuals.resize(n);
  copy_and_scale(p.b, residuals, -1.0);

  // Initialize objective value:
  double obj = compute_primal_obj(lambda);

  // Additional convergence variables:
  itr = 0;
  double last_obj = obj;

  // CD optimization loop:
  while (++itr) {
    for (int j = 0; j < d; ++j) {
      update_coordinate(j, lambda);
    }

    if (itr % 5 == 1) {
      obj = compute_primal_obj(lambda);

      // Perform logging:
      timer.pause_timing();
      double elapsed_time = timer.elapsed_time();

      bool converged = false;
      if (elapsed_time > p.max_time) {
        converged = true;
      }
      if (last_obj - obj <= p.tolerance) {
        converged = true;
      }
      last_obj = obj;

      if (p.verbose) {
        cout << "Time: " << elapsed_time
             << ", Obj: " << obj 
             << endl;
      }
      logger.log_new_point(elapsed_time, obj, converged);
      std::vector<int> nonzeros;
      for (int j = 0; j < d; ++j) {
        if (x[j] != 0.) {
          nonzeros.push_back(j);
        }
      }
      logger.log_vector<int>("nonzeros", nonzeros);
      timer.continue_timing();

      if (converged) {
        std::cout << "converged" << std::endl;
        break;
      }
    }
  }

  // Copy solution into result objects:
  for (int j = 0; j < d; ++j)
    x_result[j] = x[j];

  for (int i = 0; i < n; ++i)
    residuals_result[i] = residuals[i];

  *obj_result = obj;

  // Done!!
}


void Solver::update_coordinate(int j, double lambda) {
  const Column& col = p.A_cols[j];
  double inv_hess = col.inv_norm_sq;
  if (inv_hess < 0 || inv_hess != inv_hess) {
    return;
  }

  double grad = ip(residuals, col);
  double pre_shrink = x[j] - grad * inv_hess;
  double new_value = soft_threshold(pre_shrink, lambda * inv_hess);
  double delta = new_value - x[j];
  if (delta == 0.0) {
    return;
  }

  for (size_t ind = 0; ind < col.indices.size(); ind++) {
    residuals[col.indices[ind]] += delta * col.values[ind];
  }
  x[j] = new_value;

}

} // namespace CDL1
