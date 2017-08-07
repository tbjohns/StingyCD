#include "common.h"

#include <cmath>
#include <vector>
#include <functional>
#include <numeric>
#include <algorithm>

using std::vector;
using std::abs;
using std::accumulate;
using std::inner_product;
using std::minus;
using std::max_element;

namespace CDL1 {

double ip(const vector<double> & v, const Column & c) {
  double result = 0.0;
  for (size_t idx = 0; idx < c.indices.size(); idx++) 
    result += v[c.indices[idx]] * c.values[idx];
  return result;
}

double ip(const vector<double> & v1, const vector<double> & v2) {
  return inner_product(v1.begin(), v1.end(), v2.begin(), 0.0);
}

double sum(const vector<double> & v) {
  return accumulate(v.begin(), v.end(), 0.0); 
}

double mean(const vector<double> & v) {
  return accumulate(v.begin(), v.end(), 0.0) / v.size(); 
}

void add_scalar_to_vector(std::vector<double> & v, double scalar) {
  for (size_t i = 0; i < v.size(); ++i) {
    v[i] += scalar;
  }
}

double plus_abs(double x, double y) {
  return x + abs(y);
}

double l1(const vector<double> & v) {
  return accumulate(v.begin(), v.end(), 0.0, plus_abs);
}

double plus_sq(double x, double y) {
  return x + y * y;
}

double l2_sq(const vector<double> &v) {
  return accumulate(v.begin(), v.end(), 0.0, plus_sq);
}

double norm_dif_sq(const vector<double> &v1, const vector<double> & v2) {
  return inner_product(v1.begin(), v1.end(), v2.begin(), 0.0, plus_sq, minus<double>()); 
}

double soft_threshold(const double &value, const double &threshold) { 
  if (value > threshold)
    return value - threshold;
  else if (value < -threshold)
    return value + threshold;
  else
    return 0.0;
}

bool cmp_abs(const double &v1, const double &v2) {
  return (abs(v2) > abs(v1));
}

double max_abs(const vector<double> &values) {
  double r = *max_element(values.begin(), values.end(), cmp_abs);
  return abs(r);
}

int sign(double val) {
  return (val > 0) ? 1 : -1;
}

double sq(double val) {
  return val * val;
}

void copy_and_scale(const vector<double> &values,
                      vector<double> &target, 
                      double scale) {
  target = values;
  for (size_t ind = 0; ind < target.size(); ++ind) {
    target[ind] *= scale;
  }
}

}
