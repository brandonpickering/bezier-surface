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
  float x, y, z;
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


#include "vector.inl"


#endif
