
#include <chrono>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <stdio.h>
#include <string>
#include <thread>
#include <vector>

#include "physics.hh"
#include "system.hh"
#include "utils.hh"
#include "vec.hh"

#include <chrono>
using namespace std::chrono;
template <class T> using vecT = Vec3<T>;

int main(int argc, char *argv[]) {
  Config config;
  config.nbodies = 32768;
  config.end_time = 1000.0;
  config.timestep = 0.1;

  if (argc < 2) {
    int nbodies;
    double end_time;
    double timestep;

    std::cout << "Please enter the number of particles you want to simulate: ";
    std::cin >> nbodies;

    std::cout << "Simulation duration: ";
    std::cin >> end_time;

    std::cout << "Integration timestep: ";
    std::cin >> timestep;

    config.nbodies = nbodies;
    config.end_time = end_time;
    config.timestep = timestep;
  }

  System<vecT<float>> system;
  system.setup(config);
  std::cout << "setup done\n";

  auto start = high_resolution_clock::now();
  system.advance();
  auto stop = high_resolution_clock::now();

  std::chrono::duration<double, std::milli> fp_ms = stop - start;
  std::cout << "system.advance() duration (ms): " << fp_ms.count();
}