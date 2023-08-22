
#pragma once

#include "system.hh"
#include <vector>

// Generate disc of particles
template <class vecT, typename T>
std::vector<vecT> generate_frisbee(int n_bodies, T rad);

// Generate ring of particles
template <class vecT, typename T>
 std::vector<vecT> generate_ring(int n_bodies, T rad);

// Generate sphere of particles
template <class vecT, typename T>
std::vector<vecT> generate_sphere(int n_bodies, T rad);

// Generate hollow sphere of particles
template <class vecT, typename T>
std::vector<vecT> generate_hollow_sphere(int n_bodies, T rad);

// Generate two hollow spheres of particles
template <class vecT> std::vector<vecT> generate_two_sphere(int n_bodies);

// Generate concentric rings of particles
template <class vecT> std::vector<vecT> generate_concentric_rings(int n_bodies);

// Generate random mass
template <typename T> std::vector<T> generate_random_mass(int n_bodies, T max_mass = 1e4f);

// Calculate orbital velocity from position
template <class vecT, typename T>
std::vector<vecT> orbital_velocity(const std::vector<vecT> &pos,
                                   const T large_mass);

// Add "galaxy" to system
template <class vecT, typename T>
void add_galaxy_to_system(System<vecT> &system, const vecT &center,
                          int n_bodies, T large_mass = 3e10f);

// N rotating hollow-sphere pairs rotating around the center
template <class vecT> void rotating_n(System<vecT> &system);

// 4 rotating hollow-sphere pairs rotating around the center
template <class vecT> void rotating_4(System<vecT> &system);

#include "utils_impl.hh"
