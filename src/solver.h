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
      std::vector<bool> is_screened;
      std::vector<double> ATresiduals;

      double obj;
      int n, d;

      Timer timer;
      double obj_last_epoch;
      int itr;

      void update_coordinate(int j, double lambda);
      double compute_primal_obj(double lambda);
      void perform_screening(double lambda);

    public:

      Solver(Problem &problem);

      void solve(
        double lambda, 
        double* x, 
        double* residuals, 
        double *obj,
        const char* log_dir);



  };

}


#endif 
