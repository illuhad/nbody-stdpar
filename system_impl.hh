
#pragma once

#include <iostream>
#include <iomanip>
#include <fstream>
#include "system.hh"
#include "time_integration.hh"
#include "utils.hh"

template <class vecT> void System<vecT>::setup(const Config &config) {
  num_bodies = config.nbodies;
  end_time = config.end_time;
  timestep = config.timestep;

  //rotating_n(*this);
  rotating_4(*this);

  // Acceleration vector still neds to be initialized
  sysAcc = std::vector<vecT>(num_bodies, vecT());
}

template <class vecT> void System<vecT>::advance() {
  using T = typename vecT::value_type;
  T time = 0.0f;
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

template <class vecT> void write_points(int filenum, System<vecT> &system) {
  auto vmag = calculate_velocity_mag(system);
  std::ofstream outfile("velocity_magnitude." + std::to_string(filenum) + ".3D");
  outfile << std::setprecision(8);
  outfile << "x y z velocity\n";
  outfile << "#coordflag xyzm\n";
  for (int i = 0; i < system.sysPos.size(); i++) {
    outfile << system.sysPos[i].x << " " << system.sysPos[i].y << " "
            << system.sysPos[i].z << " " << vmag[i] << "\n";
  }
}
