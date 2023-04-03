
#pragma once

#include <algorithm>
#include <execution>
#include <fstream>
#include <iostream>
#include <random>

#include "math_functions.hh"
#include "physics.hh"
#include "utils.hh"

// global namepsace for RNGs and PI
namespace {
std::random_device rd;
std::mt19937 rg(rd());
std::uniform_real_distribution<float> dist(0, 1);
std::lognormal_distribution<float> lgdist(0.0, 1.0);
static constexpr float PI = 3.14159265358979323846;
}; // namespace

// Generate disc of particles
template <class vecT>
std::vector<vecT> generate_frisbee(int n_bodies, float rad) {
  auto randomGenerator = [=]() {
    vecT p;
    const float theta = PI * 2 * dist(rg);
    const float radius = dist(rg) * rad;
    p.x = cos(theta) * radius;
    p.y = sin(theta) * radius;
    p.z = dist(rg) * rad / 10.f;
    return p;
  };
  std::vector<vecT> particles(n_bodies, vecT());
  std::generate(particles.begin(), particles.end(), randomGenerator);
  return particles;
}

template <class vecT> std::vector<vecT> generate_ring(int n_bodies, float rad) {

  auto randomGenerator = [=]() {
    vecT p;
    const float theta = PI * 2 * dist(rg);
    const float radius = rad + dist(rg) * rad / 20.f;
    p.x = cos(theta) * radius;
    p.y = sin(theta) * radius;
    p.z = dist(rg) * rad / 100.f;
    return p;
  };
  std::vector<vecT> particles(n_bodies, vecT());
  std::generate(particles.begin(), particles.end(), randomGenerator);
  return particles;
}

template <class vecT>
std::vector<vecT> generate_sphere(int n_bodies, float rad) {

  auto randomGenerator = [=]() {
    vecT p;
    const float theta = PI * 2 * dist(rg);
    const float phi = PI * dist(rg);
    const float radius = dist(rg) * rad;
    p.x = sin(theta) * cos(phi) * radius;
    p.y = sin(theta) * sin(phi) * radius;
    p.z = cos(theta) * radius;
    return p;
  };
  std::vector<vecT> particles(n_bodies, vecT());
  std::generate(particles.begin(), particles.end(), randomGenerator);
  return particles;
}

template <class vecT>
std::vector<vecT> generate_hollow_sphere(int n_bodies, float rad) {

  auto randomGenerator = [=]() {
    vecT p;
    const float theta = PI * 2 * dist(rg);
    const float phi = PI * dist(rg);
    const float radius = rad + dist(rg) * rad / 100.f;
    p.x = sin(theta) * cos(phi) * radius;
    p.y = sin(theta) * sin(phi) * radius;
    p.z = cos(theta) * radius;
    return p;
  };

  std::vector<vecT> particles(n_bodies, vecT());
  std::generate(particles.begin(), particles.end(), randomGenerator);
  return particles;
}

template <class vecT> std::vector<vecT> generate_two_sphere(int n_bodies) {
  const vecT com1(30000.0f, 0.0f, 0.0f);
  const vecT com2(-30000.0f, 0.0f, 0.0f);

  const int half = n_bodies / 2;
  const int other_half = n_bodies - half;

  std::vector<vecT> p1 = generate_hollow_sphere<vecT>(half, 20000);
  std::vector<vecT> p2 = generate_hollow_sphere<vecT>(other_half, 20000);

  for (auto &p : p1)
    p += com1;
  for (auto &p : p2)
    p += com2;

  p1.insert(p1.end(), p2.begin(), p2.end());
  return p1;
}

template <class vecT>
std::vector<vecT> generate_concentric_rings(int n_bodies) {
  const int third = n_bodies / 3;
  const int other_third = n_bodies - third * 2;

  std::vector<vecT> p1 = generate_ring<vecT>(third, 10000);
  std::vector<vecT> p2 = generate_ring<vecT>(third, 30000);
  std::vector<vecT> p3 = generate_ring<vecT>(other_third, 50000);

  p1.insert(p1.end(), p2.begin(), p2.end());
  p1.insert(p1.end(), p3.begin(), p3.end());

  return p1;
}

template <class T> std::vector<T> generate_random_mass(int n_bodies) {

  auto randomGenerator = [=]() {
    T mass = 1.0 + lgdist(rg) * 10000.0;
    return mass;
  };
  std::vector<T> mass(n_bodies, 0.0f);
  std::generate(mass.begin(), mass.end(), randomGenerator);
  return mass;
}

template <class vecT, typename T>
std::vector<vecT> orbital_velocity(const std::vector<vecT> &pos,
                                   const T large_mass) {
  std::vector<vecT> vel(pos.size(), vecT());
  std::transform(std::execution::par_unseq, std::begin(pos), std::end(pos),
                 std::begin(vel), [=](const vecT &pos) {
                   vecT v = cross_product(pos, vecT(0, 0, 1));
                   T orbital_vel = sqrtf(large_mass / magnitude(v));
                   v = normalize(v) * orbital_vel;
                   return v;
                 });
  return vel;
}

template <class vecT>
void add_galaxy_to_system(System<vecT> &system, const vecT &center,
                          int n_bodies, float large_mass) {
  using T = typename vecT::value_type;
  std::vector<T> m = generate_random_mass<T>(n_bodies);
  std::vector<vecT> p = generate_two_sphere<vecT>(n_bodies);
  std::vector<vecT> v = orbital_velocity(p, large_mass);

  p[0] = {0, 0, 0};
  v[0] = {0, 0, 0};
  m[0] = large_mass;

  for (auto &pos : p)
    pos += center;

  std::vector<vecT> orb_vel = orbital_velocity(p, 1e13);
  for (int i = 0; i < orb_vel.size(); i++)
    v[i] += orb_vel[i];

  system.sysPos.insert(system.sysPos.end(), p.begin(), p.end());
  system.sysVel.insert(system.sysVel.end(), v.begin(), v.end());
  system.sysMss.insert(system.sysMss.end(), m.begin(), m.end());
}

template <class vecT> void colliding_galaxies(System<vecT> &system) {
  using T = typename vecT::value_type;
  const int quad = system.num_bodies / 4;
  const int last_quad = system.num_bodies - quad * 3;

  const vecT center1(400000.0f, 400000.0f, 0.0f);
  const vecT center2(-400000.0f, -400000.0f, 0.0f);
  const vecT center3(400000.0f, -400000.0f, 0.0f);
  const vecT center4(-400000.0f, 400000.0f, 0.0f);
  add_galaxy_to_system(system, center1, quad);
  add_galaxy_to_system(system, center2, quad);
  add_galaxy_to_system(system, center3, quad);
  add_galaxy_to_system(system, center4, last_quad);
  system.sysAcc = std::vector<vecT>(system.num_bodies, vecT());
}

template <class vecT> void write_points(int filenum, System<vecT> &system) {
  // get momentum
  auto momentum = calculate_momentum(system);

  // write file
  std::ofstream momfile;
  momfile.open("momentum." + std::to_string(filenum) + ".3D");
  momfile << "x y z momentum\n";
  momfile << "#coordflag xyzm\n";
  for (int i = 0; i < system.sysPos.size(); i++) {
    momfile << system.sysPos[i].x << " " << system.sysPos[i].y << " "
            << system.sysPos[i].z << " " << magnitude(momentum[i]) << "\n";
  }
  momfile.close();
}
