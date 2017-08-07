#include "logger.h"

#include <limits>
#include <iostream>
#include <sstream>

using std::string;
using std::ofstream;
using std::ostringstream;
using std::cout;
using std::endl;

namespace CDL1 {

Logger::Logger() :
      log_directory(""),
      min_time_interval(0.),
      last_log_time(0.),
      num_points_logged(0) { } 


void Logger::set_log_directory(const char* directory) { 
  log_directory = directory; 
  directory_is_valid = (log_directory.length() > 0) ? true : false;
  const char dir_char = '/';
  if (directory_is_valid && 
        log_directory[log_directory.length()-1] != dir_char) {
    log_directory.push_back(dir_char);
  }
}


void Logger::throttle_logging_with_interval(double min_time_interval) {
  this->min_time_interval = min_time_interval;
}


void Logger::log_new_point(double elapsed_time, double obj, bool force_no_throttle) {
  if (!directory_is_valid) {
    return;
  }

  throttle_point = (num_points_logged > 0) && 
                   (elapsed_time < last_log_time + min_time_interval) &&
                   (!force_no_throttle);
  if (throttle_point) {
    return;
  }

  ++num_points_logged;

  log_value<double>("time", (double) elapsed_time);
  log_value<double>("dual_obj", obj);

  last_log_time = elapsed_time;
}


void Logger::open_log_file(ofstream &log_file, const string &name) const {
  string filepath = get_filepath(name);
  log_file.open(filepath.c_str());
  log_file.precision(15);
  if (log_file.fail()) {
    cout << "Warning: " << get_file_error_message(filepath) << endl;
  }
}


string Logger::get_filepath(const string &name) const {
  return log_directory + name + "." + unsigned2string(num_points_logged);
}


string Logger::get_file_error_message(const string &filepath) const {
  return "could not open file " + filepath + " for logging";
}


string unsigned2string(unsigned num) {
  ostringstream ss;
  ss << num;
  return ss.str();
}


} // namespace BlitzML
