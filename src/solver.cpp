#include "solver.h"
#include "timer.h"
#include "one_minus_cdf_sqrt.h"

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

  thresholds.assign(d, 0.);
  thresholds2.assign(d, 0.);
  reference = residuals;
  reference_dist_sq = 0.;
  ATreference.assign(d, 0.);
  t_last_update.assign(d, 0);
  t = 0;
  sparsity_x = 0;
  bool ignore_thresholds = true;

  // CD optimization loop:
  while (++itr) {
    if (should_update_reference()) {
      update_reference_time = update_reference(lambda);
      time_reference_last_updated = timer.elapsed_time();
      ignore_thresholds = false;
    }

    // Update coordinates:
    for (int j = 0; j < d; ++j) {
      if (!ignore_thresholds && x[j] == 0.) {
        if (thresholds[j] >= reference_dist_sq) {
          // Normal skipped update:
          continue;
        }
        if (reference_dist_sq > 0.) {
          double p1 = one_minus_norm_cdf_sqrt_x_vals(thresholds[j] / reference_dist_sq * (n-1));
          double p2 = one_minus_norm_cdf_sqrt_x_vals(thresholds2[j] / reference_dist_sq * (n-1));
          double p = p1 + p2;
          if (p > 1.0) {
            // This can happen because of the way we approximate the probabilities
            p = 1.0;
          }
          double D_j = t - t_last_update[j];
          if (p * D_j < sparsity_x) {
            continue;
          }
        }
      }
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

  ++t;
  t_last_update[j] = t;

  double grad = ip(residuals, col);
  double pre_shrink = x[j] - grad * inv_hess;
  double new_value = soft_threshold(pre_shrink, lambda * inv_hess);
  double delta = new_value - x[j];
  if (delta == 0.0) {
    return;
  }

  if (x[j] == 0.) {
    ++sparsity_x; 
  } else if (new_value == 0.) {
    --sparsity_x;  
  }

  for (size_t ind = 0; ind < col.indices.size(); ind++) {
    residuals[col.indices[ind]] += delta * col.values[ind];
  }
  x[j] = new_value;

  reference_dist_sq += delta * (2 * (grad - ATreference[j]) + delta / inv_hess);
}

bool Solver::should_update_reference() {
  if (itr == 3) 
    return true;
  double time_diff = timer.elapsed_time() - time_reference_last_updated;
  if (time_diff > 5 * update_reference_time) {
    return true;
  }
  return false;
}

double Solver::update_reference(double lambda) {
  Timer update_timer;
  reference_dist_sq = 0.;
  reference = residuals;
  for (size_t j = 0; j < p.A_cols.size(); ++j) {
    ATreference[j] = ip(reference, p.A_cols[j]);

    double diff = lambda - ATreference[j];
    double thresh = sign(diff) * sq(diff) * p.A_cols[j].inv_norm_sq;

    double diff2 = lambda + ATreference[j];
    double thresh2 = sign(diff2) * sq(diff2) * p.A_cols[j].inv_norm_sq;

    thresholds[j] = min(thresh, thresh2);
    thresholds2[j] = max(thresh, thresh2);
  }
  return update_timer.elapsed_time();
}

} // namespace CDL1
