#ifndef __COMMON_H_INCLUDED__
#define __COMMON_H_INCLUDED__ 

#include <cstddef>
#include <vector>

namespace CDL1 {

  struct Column {
    std::vector<int> indices;
    std::vector<double> values;
    double inv_norm_sq;
    double inv_norm_sq_centered;

    void append(int i, double v) {
      indices.push_back(i);
      values.push_back(v);
    }  

    void cache_constants(int n) {
      double norm_sq = 0;
      double sum = 0;
      for (size_t idx = 0; idx < values.size(); idx++) {
        norm_sq += values[idx] * values[idx];
        sum += values[idx];
      }
      if (norm_sq != 0.) {
        inv_norm_sq = 1 / norm_sq;
        inv_norm_sq_centered = 1 / (norm_sq - sum*sum/n);
      } else {
        inv_norm_sq = -1;
        inv_norm_sq_centered = -1;
      }
    }
  };

  double ip(const std::vector<double> & v, const Column & c);
  double ip(const std::vector<double> & v1, const std::vector<double> & v2);
  double norm_dif_sq(const std::vector<double> &v1, const std::vector<double> & v2);

  double sum(const std::vector<double> & v);
  double mean(const std::vector<double> & v);
  void add_scalar_to_vector(std::vector<double> & v, double scalar);
  double l1(const std::vector<double> & v);
  int l0(const std::vector<double> & v);
  double l1(const double *v, const int &len);
  double l2_sq(const std::vector<double> &v);
  double max_abs(const std::vector<double> &values);
  int sign(double val);
  double sq(double val);

  void copy_and_scale(const std::vector<double> &values, 
                        std::vector<double> &target,
                        double scale);
  double soft_threshold(const double &value, const double &threshold);

}

#endif 
