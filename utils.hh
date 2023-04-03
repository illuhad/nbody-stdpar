
#pragma once

#include "system.hh"
#include <vector>

// Generate disc of particles
template <class vecT>
std::vector<vecT> generate_frisbee(int n_bodies, float rad);

// Generate ring of particles
template <class vecT> std::vector<vecT> generate_ring(int n_bodies, float rad);

// Generate sphere of particles
template <class vecT>
std::vector<vecT> generate_sphere(int n_bodies, float rad);

// Generate hollow sphere of particles
template <class vecT>
std::vector<vecT> generate_hollow_sphere(int n_bodies, float rad);

// Generate two hollow spheres of particles
template <class vecT> std::vector<vecT> generate_two_sphere(int n_bodies);

// Generate concentric rings of particles
template <class vecT> std::vector<vecT> generate_concentric_rings(int n_bodies);

// Generate random mass
template <typename T> std::vector<T> generate_random_mass(int n_bodies);

// Calculate orbital velocity from position
template <class vecT, typename T>
std::vector<vecT> orbital_velocity(const std::vector<vecT> &pos,
                                   const T large_mass);

// Add "galaxy" to system
template <class vecT>
void add_galaxy_to_system(System<vecT> &system, const vecT &center,
                          int n_bodies, float large_mass = 3e10);

// Create colliding galaxies scenario
template <class vecT> void colliding_galaxies(System<vecT> &system);

// write points.3D file
template <class vecT> void write_points(int filenum, System<vecT> &system);

#include "utils_impl.hh"
