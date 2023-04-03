
#pragma once

#include "vec.hh"
#include <vector>

struct Config {
  int shape;
  int nbodies;
  double timestep;
  double end_time;
};

template <class vecT> struct System {
  using T = typename vecT::value_type;
  // typename vecT::value_type T;
  std::vector<vecT> sysPos; // positions
  std::vector<vecT> sysVel; // velocities
  std::vector<vecT> sysAcc; // accel
  std::vector<T> sysMss;    // mass
  int num_bodies{0};
  double end_time{0.0};
  double timestep{0.0};
  double elapsed_time{0.0};
  System() {}
  void setup(const Config &config);
  void advance();
};

#include "system_impl.hh"
