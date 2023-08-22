cmake_minimum_required(VERSION 3.18 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(gravity VERSION 1.0 LANGUAGES CXX)

if (ENABLE_NVIDIA_GPU)
  enable_language(CUDA)
  set(CMAKE_CXX_COMPILER "nvc++")
  set(DEVICE_CXX_FLAGS " -stdpar=gpu -fast" PARENT_SCOPE)
else()
  find_package(TBB REQUIRED)
  set(CMAKE_CXX_COMPILER "g++")
  set(DEVICE_CXX_FLAGS  " -Ofast -march=native" PARENT_SCOPE)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
  message(STATUS "Adding device specific compiler flags to CMAKE_CXX_FLAGS: '${DEVICE_CXX_FLAGS}'")
  string(APPEND CMAKE_CXX_FLAGS ${DEVICE_CXX_FLAGS})
endif()

set(nbody_hh_files
math_functions.hh
math_functions_impl.hh
physics.hh
physics_impl.hh
utils.hh
utils_impl.hh
system.hh
system_impl.hh
time_integration.hh
time_integration_impl.hh
vec.hh
vec_impl.hh
)

add_executable(grav main.cc ${nbody_hh_files})

if (ENABLE_NVIDIA_GPU)
  add_compile_definitions (ENABLE_CUDA)
  target_compile_options(grav PRIVATE -stdpar)
  target_link_options(grav PRIVATE -stdpar)
else()
  find_package(TBB REQUIRED)
  target_link_libraries(grav PRIVATE TBB::tbb)
endif()

install(TARGETS grav)
