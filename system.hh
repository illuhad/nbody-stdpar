
#pragma once

#include <vector>

struct Config {
#if defined(ENABLE_CUDA) || defined(ENABLE_ACPP)
  int device{1};
#else
  int device{2};
#endif
  int nbodies{-1};
  int shape{-1};
  float timestep;
  float end_time;
};

template <class vecT> struct System {
  using T = typename vecT::value_type;
  std::vector<vecT> sysPos; // positions
  std::vector<vecT> sysVel; // velocities
  std::vector<vecT> sysAcc; // accel
  std::vector<T> sysMss;    // mass
  int num_bodies{0};
  T end_time{0.0};
  T timestep{0.0};
  T elapsed_time{0.0};
  System() {}
  void setup(Config &config);
  void advance();
};

// write points.3D file
template <class vecT> void write_points(int filenum, System<vecT> &system);

#include "system_impl.hh"
