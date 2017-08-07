#ifndef __SOLVER_H_INCLUDED__
#define __SOLVER_H_INCLUDED__ 

#include "common.h"
#include "problem.h"
#include "logger.h"
#include "timer.h"
#include <string>
#include <vector>

namespace CDL1 {

  class Solver {

    private:

      const Problem &p;

      std::vector<double> residuals;
      std::vector<double> x;

      std::vector<double> reference;
      std::vector<double> ATreference;
      std::vector<double> thresholds;
      std::vector<double> thresholds2;
      std::vector<size_t> t_last_update;
      size_t t;
      int update_threshold_next;
      int update_threshold;
      int sparsity_x;
      Timer timer;
      double update_reference_time;
      double time_reference_last_updated;
      double reference_dist_sq;
      double obj_last_epoch;
      int itr;

      double update_reference(double lambda);
      bool should_update_reference();
      void update_coordinate(int j, double lambda);

    public:

      Solver(Problem &problem);

      void solve(
        double lambda, 
        double* x, 
        double* residuals, 
        double *obj,
        const char* log_dir);

      double compute_primal_obj(double lambda);


  };

}


#endif 
