
#pragma once

#include "system.hh"
#include <vector>


// Calculate all-pairs forces
template <class vecT> void accumulate_forces(System<vecT> &system);

// Calculates acceleration on particle pos1 due to interaction with particle
// pos2
template <class vecT, typename T>
vecT acceleration(const vecT &pos1, const vecT &pos2, const T mass2);

// Update system velocities
template <class vecT, typename T>
void update_velocities(System<vecT> &system, T timestep);

// Update system positions
template <class vecT, typename T>
void update_positions(System<vecT> &system, T timestep);

// Calculate momentum from system velocity and mass
template <class vecT>
std::vector<vecT> calculate_momentum(System<vecT> &system);

// Calculate magnitude of vector velocities
template <class vecT>
auto calculate_velocity_mag(System<vecT> &system);

#include "physics_impl.hh"
