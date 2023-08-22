
#pragma once

#include <execution>
#include <vector>
#include "physics.hh"

// forward Euler
template <class vecT> void integrate_euler(System<vecT> &system) {
  // Acc(t+dt) = f(Pos(t))
  accumulate_forces(system, system.sysAcc);
  // Vel(t+dt) = Vel(t) + Acc(t+dt) * dt
  update_velocities(system, system.timestep);
  // Pos(t+dt) = Pos(t) + Vel(t+dt) * dt
  update_positions(system, system.timestep);
}

// Velocity Verlet 4 step
template <class vecT> void integrate_verlet4(System<vecT> &system) {
  const float dt{static_cast<float>(system.timestep)};
  const float half_dt{dt / 2};
  // Vel(t+dt/2) = Vel(t) + 0.5 * dt * Acc(t)
  update_velocities(system, half_dt);
  // Pos(t+dt) = Pos(t) + Vel(t+dt/2) * dt
  update_positions(system, dt);
  // Acc(t+dt) = f(Pos(t+dt))
  accumulate_forces(system, system.sysAcc);
  // Vel(t+dt) = Vel(t+dt/2) + 0.5 * dt * Acc(t+dt)
  update_velocities(system, half_dt);
}

// Velocity Verlet 3 step
template <class vecT> void integrate_verlet3(System<vecT> &system) {
  const float dt{static_cast<float>(system.timestep)};
  const float half_dt{dt / 2};
  const float half_dtdt{dt * dt / 2};
  const int sys_size = system.sysPos.size();

  std::vector<int> sys_i(sys_size, 0);
  std::iota(std::begin(sys_i), std::end(sys_i), 0);

  // Pos(t+dt) = Pos(t) + Vel(t) * dt + 0.5 * dt * dt * Acc(t)
  {
    vecT *posptr = system.sysPos.data();
    vecT const *velptr = system.sysVel.data();
    vecT const *accptr = system.sysAcc.data();
    std::for_each(std::execution::par_unseq, std::begin(sys_i), std::end(sys_i),
                  [=](int i) {
                    posptr[i] += velptr[i] * dt;
                    posptr[i] += accptr[i] * half_dtdt;
                  });
  }

  // Acc(t+dt) = f(Pos(t+dt))
  std::vector<vecT> accel(sys_size, vecT());
  { accumulate_forces(system, accel); }

  // Vel(t+dt) = Vel(t) + (Acc(t) + Acc(t+dt)) * dt * 0.5
  {
    vecT *velptr = system.sysVel.data();
    vecT *accptr = system.sysAcc.data();
    vecT const *newacc = accel.data();
    std::for_each(std::execution::par_unseq, std::begin(sys_i), std::end(sys_i),
                  [=](int i) {
                    accptr[i] += newacc[i];
                    accptr[i] *= half_dt;
                    velptr[i] += accptr[i];
                    accptr[i] = newacc[i];
                  });
  }
}
