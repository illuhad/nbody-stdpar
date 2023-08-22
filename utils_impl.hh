
#pragma once

#include <algorithm>
#include <execution>
#include <random>
#include "math_functions.hh"
#include "utils.hh"

// global namepsace for RNGs and PI
namespace {
std::random_device rd;
std::mt19937 rg(rd());
std::uniform_real_distribution<float> dist(-1.f, 1.f);
std::lognormal_distribution<float> lgdist(0.0f, 1.0f);
static constexpr float PI = 3.14159265358979323846f;
}; // namespace


// Generate disc of particles
template <class vecT, typename T>
std::vector<vecT> generate_frisbee(int n_bodies, T rad) {
  auto randomGenerator = [=]() {
    vecT p;
    const T theta = PI * 2 * dist(rg);
    const T radius = dist(rg) * rad;
    p.x = cos(theta) * radius;
    p.y = sin(theta) * radius;
    p.z = dist(rg) * rad / 10.f;
    return p;
  };
  std::vector<vecT> particles(n_bodies, vecT());
  std::generate(particles.begin(), particles.end(), randomGenerator);
  return particles;
}


template <class vecT, typename T>
std::vector<vecT> generate_ring(int n_bodies, T rad) {
  auto randomGenerator = [=]() {
    vecT p;
    const T theta = PI * 2 * dist(rg);
    const T radius = rad + dist(rg) * rad / 20.f;
    p.x = cos(theta) * radius;
    p.y = sin(theta) * radius;
    p.z = dist(rg) * rad / 100.f;
    return p;
  };
  std::vector<vecT> particles(n_bodies, vecT());
  std::generate(particles.begin(), particles.end(), randomGenerator);
  return particles;
}


template <class vecT, typename T>
std::vector<vecT> generate_sphere(int n_bodies, T rad) {
  auto randomGenerator = [=]() {
    vecT p;
    const T theta = PI * 2 * dist(rg);
    const T phi = PI * dist(rg);
    const T radius = dist(rg) * rad;
    p.x = sin(theta) * cos(phi) * radius;
    p.y = sin(theta) * sin(phi) * radius;
    p.z = cos(theta) * radius;
    return p;
  };
  std::vector<vecT> particles(n_bodies, vecT());
  std::generate(particles.begin(), particles.end(), randomGenerator);
  return particles;
}


template <class vecT, typename T>
std::vector<vecT> generate_hollow_sphere(int n_bodies, T rad) {
  auto randomGenerator = [=]() {
    vecT p;
    const T theta = PI * 2 * dist(rg);
    const T phi = PI * dist(rg);
    const T radius = rad + dist(rg) * rad / 100.f;
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
  using T = typename vecT::value_type;
  const vecT com1(30000.0f, 0.0f, 0.0f);
  const vecT com2(-30000.0f, 0.0f, 0.0f);
  const int half = n_bodies / 2;
  const int other_half = n_bodies - half;
  std::vector<vecT> p1 = generate_hollow_sphere<vecT>(half, 20000.f);
  std::vector<vecT> p2 = generate_hollow_sphere<vecT>(other_half, 20000.f);
  std::transform(std::execution::par_unseq,
                p1.begin(), p1.end(), p1.begin(),
                [=](auto& p) { return p + com1; });
  std::transform(std::execution::par_unseq,
                p2.begin(), p2.end(), p2.begin(),
                [=](auto& p) { return p + com2; });
  p1.insert(p1.end(), p2.begin(), p2.end());
  return p1;
}


template <class vecT>
std::vector<vecT> generate_concentric_rings(int n_bodies) {
  const int third = n_bodies / 3;
  const int other_third = n_bodies - third * 2;
  std::vector<vecT> p1 = generate_ring<vecT>(third, 10000.f);
  std::vector<vecT> p2 = generate_ring<vecT>(third, 30000.f);
  std::vector<vecT> p3 = generate_ring<vecT>(other_third, 50000.f);
  p1.insert(p1.end(), p2.begin(), p2.end());
  p1.insert(p1.end(), p3.begin(), p3.end());
  return p1;
}


template <typename T> std::vector<T> generate_random_mass(int n_bodies, T max_mass) {
  auto randomGenerator = [=]() {
    T mass = 1.0f + lgdist(rg) * max_mass;
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
                   vecT v = cross_product(pos, vecT(0.f, 0.f, 1.f));
                   T orbital_vel = sqrtf(large_mass / magnitude(v));
                   v = normalize(v) * orbital_vel;
                   return v;
                 });
  return vel;
}


// create "galaxy" from two spherical shells that orbit a larger mass
template <class vecT, typename T>
void add_galaxy_to_system(System<vecT> &system, const vecT &center,
                          int n_bodies, T large_mass) {
  // get random mass distribution, positions, and orbital velocities
  std::vector<T> m = generate_random_mass<T>(n_bodies);
  // arrange as two hollow spheres centered and opposed about a large mass
  std::vector<vecT> p = generate_two_sphere<vecT>(n_bodies);
  // calculate rotational velocity of two-sphere system about 0,0,0 (local) coordinate
  std::vector<vecT> v = orbital_velocity(p, large_mass);

  // set first position of two-sphere system to center of galaxy
  p[0] = {0.f, 0.f, 0.f};
  v[0] = {0.f, 0.f, 0.f};
  m[0] = large_mass;

  // shift two-sphere positions by center coordinate
  std::transform(std::execution::par_unseq,
                p.begin(), p.end(), p.begin(),
                [=](auto& pos) { return pos + center; });

  // insert two-sphere into system vectors
  system.sysPos.insert(system.sysPos.end(), p.begin(), p.end());
  system.sysVel.insert(system.sysVel.end(), v.begin(), v.end());
  system.sysMss.insert(system.sysMss.end(), m.begin(), m.end());
}


// create system with 4 "galaxies" orbiting a large mass
template <class vecT> void rotating_4(System<vecT> &system) {
  // split nbodies into 4 groups
  const int quad = system.num_bodies / 4;
  const int last_quad = system.num_bodies - quad * 3;

  // assign subgroup centers at corners
  const vecT center1(400000.0f, 400000.0f, 0.0f);
  const vecT center2(-400000.0f, -400000.0f, 0.0f);
  const vecT center3(400000.0f, -400000.0f, 0.0f);
  const vecT center4(-400000.0f, 400000.0f, 0.0f);

  // add "galaxies" to system
  add_galaxy_to_system(system, center1, quad, 1.e10f);
  add_galaxy_to_system(system, center2, quad, 1.e10f);
  add_galaxy_to_system(system, center3, quad, 1.e10f);
  add_galaxy_to_system(system, center4, last_quad, 1.e10f);

  // calculate orbital velocity of system about global 0,0,0 coordinate
  // add it to system velocity
  std::vector<vecT> orb_vel = orbital_velocity(system.sysPos, 1e13f);
  std::transform(std::execution::par_unseq,
                orb_vel.begin(), orb_vel.end(),
                system.sysVel.begin(), system.sysVel.begin(),
                [=](const auto& ov, auto& sv) { return sv + ov; });

  // set last position to center of system
  system.sysPos[system.num_bodies-1] = {0.f, 0.f, 0.f};
  system.sysVel[system.num_bodies-1] = {0.f, 0.f, 0.f};
  system.sysMss[system.num_bodies-1] = 1.e13f;
}


// create system with many "galaxies" on a disc
template <class vecT> void rotating_n(System<vecT> &system) {
  using T = typename vecT::value_type;
  const int num_galaxies = 64;
  const int group = system.num_bodies / num_galaxies;
  const int last_group = system.num_bodies - group * (num_galaxies-1);
  auto randomGenerator = [=]() {
    vecT p;
    const T rad = 10000000.0f;
    const T theta = PI * 2 * dist(rg);
    const T radius = dist(rg) * rad;
    p.x = cos(theta) * radius;
    p.y = sin(theta) * radius;
    p.z = dist(rg) * rad / 10.f;
    return p;
  };

  // create symmetry about center
  // first assign half of the galaxies randomly
  std::vector<vecT> center(num_galaxies, vecT());
  std::generate(center.begin(), center.begin()+num_galaxies/2, randomGenerator);
  // then blance with the other half of the galaxies
  // for each randomly placed galaxy, assign a galaxy with opposite coordinates
  for (int i = 0; i < num_galaxies/2; i++)
    center[i+num_galaxies/2] = {-center[i].x, -center[i].y, -center[i].z};
  // Add all galaxies to system.
  for (int i = 0; i < center.size()-1; i++)
    add_galaxy_to_system(system, center[i], group, 1e10f);
  // last group could be a different size than prior groups
  add_galaxy_to_system(system, center[center.size()-1], last_group, 1e10f);

  // calculate orbital velocity of system about global 0,0,0 coordinate
  // spaghettification if mass provided to orbital_valocity() is smaller than mass of center position
  std::vector<vecT> orb_vel = orbital_velocity(system.sysPos, 5e14f);
  std::transform(std::execution::par_unseq,
                orb_vel.begin(), orb_vel.end(),
                system.sysVel.begin(), system.sysVel.begin(),
                [=](const auto& ov, auto& sv) { return sv + ov; });

  // set last position to center of system
  system.sysPos[system.num_bodies-1] = {0.f, 0.f, 0.f};
  system.sysVel[system.num_bodies-1] = {0.f, 0.f, 0.f};
  system.sysMss[system.num_bodies-1] = 1e15f;
}
