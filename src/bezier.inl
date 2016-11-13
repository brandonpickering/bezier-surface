#ifndef _BEZIER_INL
#define _BEZIER_INL


inline vec3f bezier2_interp(const vec3f *ctrls, float u) {
  vec3f p1 = (1-u) * ctrls[0] + u * ctrls[1];
  vec3f p2 = (1-u) * ctrls[1] + u * ctrls[2];
  return (1-u) * p1 + u * p2;
}

inline vec3f bezier_interp(const vec3f *ctrls, float u) {
  vec3f p1 = (1-u) * ctrls[0] + u * ctrls[1];
  vec3f p2 = (1-u) * ctrls[1] + u * ctrls[2];
  vec3f p3 = (1-u) * ctrls[2] + u * ctrls[3];
  vec3f p12 = (1-u) * p1 + u * p2;
  vec3f p23 = (1-u) * p2 + u * p3;
  return (1-u) * p12 + u * p23;
}

inline vec3f bezier_deriv(const vec3f *ctrls, float u) {
  vec3f dctrls[3] = {
    ctrls[1] - ctrls[0],
    ctrls[2] - ctrls[1],
    ctrls[3] - ctrls[2],
  };
  return bezier2_interp(&dctrls[0], u);
}

inline vec3f bezier_interp(const vec3f *ctrls, float u, float v) {
  vec3f vctrls[4];
  for (int i = 0; i < 4; ++i)
    vctrls[i] = bezier_interp(ctrls + 4*i, u);
  return bezier_interp(&vctrls[0], v);
}

inline vec3f bezier_deriv_u(const vec3f *ctrls, float u, float v) {
  vec3f uctrls[4];
  for (int i = 0; i < 4; ++i) {
    vec3f vctrlsi[4];
    for (int j = 0; j < 4; ++j)
      vctrlsi[j] = ctrls[4*j + i];
    uctrls[i] = bezier_interp(&vctrlsi[0], v);
  }
  return bezier_deriv(&uctrls[0], u);
}

inline vec3f bezier_deriv_v(const vec3f *ctrls, float u, float v) {
  vec3f vctrls[4];
  for (int i = 0; i < 4; ++i)
    vctrls[i] = bezier_interp(ctrls + 4*i, u);
  return bezier_deriv(&vctrls[0], v);
}

inline vec3f bezier_normal(const vec3f *ctrls, float u, float v) {
  vec3f ddu = bezier_deriv_u(ctrls, u, v);
  vec3f ddv = bezier_deriv_v(ctrls, u, v);
  return normalize(cross(ddu, ddv));
}


#endif
