
#pragma once

#include "math_functions.hh"

template <typename vecT> vecT cross_product(const vecT &v1, const vecT &v2) {
  vecT result;
  result.x = v1.y * v2.z - v2.y * v1.z;
  result.y = v1.z * v2.x - v2.z * v1.x;
  result.z = v1.x * v2.y - v2.x * v1.y;
  return result;
}

template <typename vecT> auto dot_product(const vecT &v1, const vecT &v2) {
  return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

template <typename vecT> auto dot_product(const vecT &v) {
  return v.x * v.x + v.y * v.y + v.z * v.z;
}

template <typename vecT> auto magnitude(const vecT &v) {
  return sqrtf(dot_product(v));
}

template <typename vecT> vecT normalize(const vecT &v) {
  auto len = magnitude(v);
  vecT result = v / len;
  return result;
}
