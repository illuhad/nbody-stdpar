
#pragma once

// Cross product of v1 and v2
template <typename vecT> vecT cross_product(const vecT &v1, const vecT &v2);

// Dot product of v1 and v2
template <typename vecT> auto dot_product(const vecT &v1, const vecT &v2);

// Calculates the squared magnitude of v
template <typename vecT> auto dot_product(const vecT &v);

// Magnitude of v
template <typename vecT> auto magnitude(const vecT &v);

// Normalize v
template <typename vecT> vecT normalize(const vecT &v);

#include "math_functions_impl.hh"
