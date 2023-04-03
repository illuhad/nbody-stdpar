
#pragma once

#include "system.hh"
#include <vector>

// Calculate all-pairs forces
template <typename vecT> void accumulate_forces(System<vecT> &system);

// Calculates acceleration on particle pos1 due to interaction with particle
// pos2
template <typename vecT, typename T>
vecT acceleration(const vecT &pos1, const vecT &pos2, const T mass2);

// Update system velocities
template <typename vecT>
void update_velocities(System<vecT> &system, double timestep);

// Update system positions
template <typename vecT>
void update_positions(System<vecT> &system, double timestep);

// Calculate momentum from system velocity and mass
template <typename vecT>
std::vector<vecT> calculate_momentum(System<vecT> &system);

#include "physics_impl.hh"
