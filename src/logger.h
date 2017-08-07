#pragma once

#include "common.h"

#include <string>
#include <fstream>
#include <iostream>

namespace CDL1 {

class Logger {

  public:
    Logger();

    void set_log_directory(const char* directory);

    void throttle_logging_with_interval(double min_time_interval);

    void log_new_point(double elapsed_time, double obj, bool force_no_throttle=false);

    void open_log_file(std::ofstream &log_file, const std::string &name) const;

    template <typename T>
    void log_value(std::string name, T value) const {
      if (throttle_point || !directory_is_valid) {
        return;
      }

      std::ofstream log_file;
      open_log_file(log_file, name);
      if (log_file.fail()) {
        return;
      }

      log_file << value << std::endl;
      log_file.close();
    }

    template <typename T>
    void log_vector(std::string name, const std::vector<T> &vec) const {
      if (throttle_point || !directory_is_valid) {
        return;
      }

      std::ofstream log_file;
      open_log_file(log_file, name);
      if (log_file.fail()) {
        return;
      }

      for (size_t ind = 0; ind < vec.size(); ++ind) {
        log_file << vec[ind] << std::endl;
      }

      log_file.close();
    }


  private:
    std::string log_directory;
    double min_time_interval;
    double last_log_time;
    unsigned num_points_logged;
    bool throttle_point;
    bool directory_is_valid;

    std::string get_filepath(const std::string &name) const;
    std::string get_file_error_message(const std::string &filepath) const;
};

std::string unsigned2string(unsigned num);

} // namespace BlitzML
