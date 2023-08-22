
#include <chrono>
#include <iostream>
#include "system.hh"
#include "vec.hh"

template <typename T> using vecT = Vec3<T>;

int main(int argc, char *argv[]) {
  Config config;
  config.end_time = 10000.0;
  config.timestep = 1.0;

  if (argc > 1) {
    std::cout << "Manual parameter entry mode\n";
    std::cout << "Please enter the number of particles you want to simulate (min 64): ";
    std::cin >> config.nbodies;

    std::cout << "Please enter the galaxy shape to create (1 or 2): ";
    std::cin >> config.shape;

    std::cout << "Please enter the simulation duration: ";
    std::cin >> config.end_time;

    std::cout << "Please enter the integration timestep (should be a divisor of duration): ";
    std::cin >> config.timestep;
  }

  System<vecT<float>> system;
  system.setup(config);
  std::cout << "setup done\n";

  auto start = std::chrono::high_resolution_clock::now();
  system.advance();
  auto stop = std::chrono::high_resolution_clock::now();

  std::chrono::duration<double, std::milli> fp_ms = stop - start;
  std::cout << "system.advance() duration (ms): " << fp_ms.count() << std::endl;
}