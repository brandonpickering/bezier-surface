#ifndef _VECTOR_INL
#define _VECTOR_INL


#include <cmath>

#include "common.hpp"


inline vec3f vec(float x, float y, float z) {
  return {x, y, z};
}

inline std::string str(vec3f v) {
  return stringf("(%f, %f, %f)", v.x, v.y, v.z);
}

inline float magnitude(vec3f v) {
  return sqrt(dot(v, v));
}

inline vec3f normalize(vec3f v) {
  float mag = magnitude(v);
  if (mag == 0)
    return v;
  return v / mag;
}

inline float dot(vec3f v, vec3f w) {
  return v.x*w.x + v.y*w.y + v.z*w.z;
}

inline vec3f cross(vec3f v, vec3f w) {
  vec3f r;
  r.x = v.y*w.z - v.z*w.y;
  r.y = v.z*w.x - v.x*w.z;
  r.z = v.x*w.y - v.y*w.x;
  return r;
}

inline vec3f operator+(vec3f v, vec3f w) {
  return {v.x + w.x, v.y + w.y, v.z + w.z};
}

inline vec3f operator-(vec3f v, vec3f w) {
  return {v.x - w.x, v.y - w.y, v.z - w.z};
}

inline vec3f operator-(vec3f v) {
  return {-v.x, -v.y, -v.z};
}

inline vec3f operator*(vec3f v, float s) {
  return {v.x*s, v.y*s, v.z*s};
}

inline vec3f operator*(float s, vec3f v) {
  return {s*v.x, s*v.y, s*v.z};
}

inline vec3f operator/(vec3f v, float s) {
  return {v.x/s, v.y/s, v.z/s};
}

inline vec3f operator*(vec3f v, vec3f w) {
  return {v.x*w.x, v.y*w.y, v.z*w.z};
}


#endif
