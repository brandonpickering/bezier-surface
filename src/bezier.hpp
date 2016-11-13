#ifndef _BEZIER_HPP
#define _BEZIER_HPP


#include "common.hpp"


vec3f bezier2_interp(const vec3f *ctrls, float u);

vec3f bezier_interp(const vec3f *ctrls, float u);
vec3f bezier_deriv(const vec3f *ctrls, float u);

vec3f bezier_interp(const vec3f *ctrls, float u, float v);
vec3f bezier_deriv_u(const vec3f *ctrls, float u, float v);
vec3f bezier_deriv_v(const vec3f *ctrls, float u, float v);
vec3f bezier_normal(const vec3f *ctrls, float u, float v);


#include "bezier.inl"


#endif
