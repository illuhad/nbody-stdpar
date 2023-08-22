
#pragma once

#include "system.hh"

// forward Euler
// Acc(t+dt) = f(Pos(t))
// Vel(t+dt) = Vel(t) + Acc(t+dt) * dt
// Pos(t+dt) = Pos(t) + Vel(t+dt) * dt
template <class vecT> void integrate_euler(System<vecT> &system);

// Velocity Verlet 4 step
// Vel(t+dt/2) = Vel(t) + 0.5 * dt * Acc(t)
// Pos(t+dt) = Pos(t) + Vel(t+dt/2) * dt
// Acc(t+dt) = f(Pos(t+dt))
// Vel(t+dt) = Vel(t+dt/2) + 0.5 * dt * Acc(t+dt)
template <class vecT> void integrate_verlet4(System<vecT> &system);

// Velocity Verlet 3 step
// Pos(t+dt) = Pos(t) + Vel(t) * dt + 0.5 * dt * dt * Acc(t)
// Acc(t+dt) = f(Pos(t+dt))
// Vel(t+dt) = Vel(t) + 0.5 * dt * (Acc(t)+Acc(t+dt))
template <class vecT> void integrate_verlet3(System<vecT> &system);

#include "time_integration_impl.hh"
