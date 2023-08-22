
#pragma once

#include <algorithm>
#include <execution>
#include <math.h>
#include <vector>

#include "math_functions.hh"
#include "physics.hh"

namespace {
inline float inv_sqrt(const float x)
{
#ifdef ENABLE_CUDA
  return rsqrtf(x);
#else
  __m128 y = _mm_set_ss(x); y = _mm_rsqrt_ss(y); return _mm_cvtss_f32(y);
#endif
}
}

// Calculate all-pairs forces
// use transform to parallelize outer loop
// each thread_i runs a sequential inner loop
// over all particles, summing forces on particle_i
template <typename vecT>
void accumulate_forces(System<vecT> &system, std::vector<vecT> &accel) {
  using T = typename vecT::value_type;
  const int sys_size{system.sysPos.size()};

  T const *mssptr = system.sysMss.data();
  vecT const *posptr = system.sysPos.data();
  std::transform(std::execution::par_unseq, std::begin(system.sysPos),
                 std::end(system.sysPos), std::begin(accel),
                 [=](const vecT &pos) {
                   vecT acc;
                   for (int j = 0; j < sys_size; j++)
                     acc += acceleration(pos, posptr[j], mssptr[j]);
                   return acc;
                 });
}

// Calculates acceleration on particle i
// due to interaction with particle j
// Force equation
// F_i = G*m_i*m_j / (r_ij^2)^(1/2)
// Rearrange to solve for acceleration vector with G = 1
// A_i = m_j * r_ij / (|r_ij|^2)^(3/2)
template <typename vecT, typename T>
vecT acceleration(const vecT &pos1, const vecT &pos2, const T mass2) {
  vecT rel_dist = pos2 - pos1; // relative distance
  T rd_sq{0.00001};            // rel_dist^2, initialize with softening length
  rd_sq += dot_product(rel_dist);
  const T rd_mag = inv_sqrt(rd_sq); // 1 / (rel_dist dot rel_dist)^(1/2)
  const T impulse = mass2 * rd_mag * rd_mag * rd_mag; // m_j / |rel_dist|^3
  rel_dist *= impulse;                                // acceleration vector
  return rel_dist;
}

template <typename vecT>
void update_velocities(System<vecT> &system, double timestep) {
  const float dt{static_cast<float>(timestep)};
  std::transform(std::execution::par_unseq, std::begin(system.sysAcc),
                 std::end(system.sysAcc), std::begin(system.sysVel),
                 std::begin(system.sysVel), [=](const vecT &accel, vecT &vel) {
                   vel += accel * dt;
                   return vel;
                 });
}

template <typename vecT>
void update_positions(System<vecT> &system, double timestep) {
  const float dt{static_cast<float>(timestep)};
  std::transform(std::execution::par_unseq, std::begin(system.sysVel),
                 std::end(system.sysVel), std::begin(system.sysPos),
                 std::begin(system.sysPos), [=](const vecT &vel, vecT &pos) {
                   pos += vel * dt;
                   return pos;
                 });
}

template <typename vecT>
std::vector<vecT> calculate_momentum(System<vecT> &system) {
  std::vector<vecT> momentum(system.sysVel.size(), vecT());
  std::transform(std::execution::par_unseq, std::begin(system.sysVel),
                 std::end(system.sysVel), std::begin(system.sysMss),
                 std::begin(momentum),
                 [=](vecT vel, float mss) { return vel * mss; });
  return momentum;
}