#ifndef _COMMON_HPP
#define _COMMON_HPP


#include <cstdint>
#include <string>


template <typename... T>
std::string stringf(const char *format, T... args) {
  size_t len = snprintf(nullptr, 0, format, args...);
  char *buffer = new char[len + 1];
  snprintf(buffer, len + 1, format, args...);
  std::string result(buffer);
  delete[] buffer;
  return result;
}



struct vec3f {
  union {
    float data[3];
    struct { float x, y, z; };
  };
};

vec3f vec(float x, float y, float z);

std::string str(vec3f);

float magnitude(vec3f);
vec3f normalize(vec3f);
float dot(vec3f, vec3f);
vec3f cross(vec3f, vec3f);

vec3f operator+(vec3f, vec3f);
vec3f operator-(vec3f, vec3f);
vec3f operator-(vec3f);

vec3f operator*(vec3f, float);
vec3f operator*(float, vec3f);
vec3f operator/(vec3f, float);

vec3f operator*(vec3f, vec3f); // Hadamard product



inline vec3f bezier_interp(const vec3f *ctrls, float u) {
  vec3f p1 = (1-u) * ctrls[0] + u * ctrls[1];
  vec3f p2 = (1-u) * ctrls[1] + u * ctrls[2];
  vec3f p3 = (1-u) * ctrls[2] + u * ctrls[3];
  vec3f p12 = (1-u) * p1 + u * p2;
  vec3f p23 = (1-u) * p2 + u * p3;
  return (1-u) * p12 + u * p23;
}

inline vec3f bezier_interp(const vec3f *ctrls, float u, float v) {
  vec3f ctrls1[4];
  for (int i = 0; i < 4; ++i)
    ctrls1[i] = bezier_interp(ctrls + 4*i, u);
  return bezier_interp(ctrls1, v);
}


#include "vector.inl"


#endif
