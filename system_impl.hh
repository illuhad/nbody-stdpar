
#pragma once

#include "system.hh"
#include "time_integration.hh"
#include "utils.hh"

template <class vecT> void System<vecT>::setup(const Config &config) {
  num_bodies = config.nbodies;
  end_time = config.end_time;
  timestep = config.timestep;

  colliding_galaxies(*this);

  sysPos[1] = {0, 0, 0};
  sysVel[1] = {0, 0, 0};
  sysMss[1] = 1.e13;
}

template <typename vecT> void System<vecT>::advance() {
  double time = 0.0;
  int cnt = 0;
  int filenum = 0;
  write_points(filenum++, *this); // initial
  while (time <= end_time) {
    integrate_verlet4(*this);
    time += timestep;
    ++cnt;
    if (cnt % 20 == 0) {
      std::cout << "writing file at time: " << time << "\n";
      write_points(filenum, *this); // every 20 timestep
      ++filenum;
    }
  }
}
