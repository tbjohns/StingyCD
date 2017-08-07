#pragma once

#include "common.h"

namespace CDL1 {

double get_time();

class Timer {
  public:
    Timer();
    void reset();
    double elapsed_time();
    void pause_timing();
    void continue_timing();

  private:
    double absolute_start_time;
    double absolute_pause_time;
    double pause_accum;
    bool is_paused;
};

} // namespace CDL1

