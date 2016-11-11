#include <vector>

#include <GLFW/glfw3.h>

#include "common.hpp"
#include "render.hpp"


static void send_gl(vec3f v) {
  glVertex3f(v.x, v.y, v.z);
}


void render_u(const std::vector<vec3f> &patch, float step) {
  glColor3f(0, 0, 0);
  for (float u0 = 0; u0 < 1.0f; u0 += step) {
    for (float v0 = 0; v0 < 1.0f; v0 += step) {
      float u1 = std::min(u0 + step, 1.0f);
      float v1 = std::min(v0 + step, 1.0f);

      glColor3f((float) rand() / RAND_MAX, (float) rand() / RAND_MAX, (float) rand() / RAND_MAX);
      glBegin(GL_QUADS);
      send_gl(bezier_interp(patch.data(), u0, v0));
      send_gl(bezier_interp(patch.data(), u1, v0));
      send_gl(bezier_interp(patch.data(), u1, v1));
      send_gl(bezier_interp(patch.data(), u0, v1));
      glEnd();
    }
  }
}

static void render_ad(const vec3f *ctrls, float tolerance,
                      float u0, float v0,
                      float u1, float v1,
                      float u2, float v2) {
  float cs[][2] = { {u0,v0}, {u1,v1}, {u2,v2} };
  float ms[][2] = {
    {(u1+u2)/2, (v1+v2)/2},
    {(u0+u2)/2, (v0+v2)/2},
    {(u0+u1)/2, (v0+v1)/2},
  };
  vec3f vs[] = {
    bezier_interp(ctrls, u0, v0),
    bezier_interp(ctrls, u1, v1),
    bezier_interp(ctrls, u2, v2),
  };
  bool good[3];
  int ngood = 0;

  for (int i = 0; i < 3; ++i) {
    vec3f tv = bezier_interp(ctrls, ms[i][0], ms[i][1]);
    vec3f lv = 0.5f * (vs[(i+1)%3] + vs[(i+2)%3]);

    good[i] = magnitude(tv - lv) <= tolerance;
    if (good[i]) ngood += 1;
  }

  if (ngood == 2) {
    int i = !good[0] ? 0 : !good[1] ? 1 : 2;
    render_ad(ctrls, tolerance, cs[i][0], cs[i][1],
                                cs[(i+1)%3][0], cs[(i+1)%3][1],
                                ms[i][0], ms[i][1]);
    render_ad(ctrls, tolerance, cs[i][0], cs[i][1],
                                cs[(i+2)%3][0], cs[(i+2)%3][1],
                                ms[i][0], ms[i][1]);

  } else if (ngood == 1) {
    int i = good[0] ? 0 : good[1] ? 1 : 2;
    render_ad(ctrls, tolerance, cs[(i+1)%3][0], cs[(i+1)%3][1],
                                cs[(i+2)%3][0], cs[(i+2)%3][1],
                                ms[(i+1)%3][0], ms[(i+1)%3][1]);
    render_ad(ctrls, tolerance, cs[(i+1)%3][0], cs[(i+1)%3][1],
                                ms[(i+1)%3][0], ms[(i+1)%3][1],
                                ms[(i+2)%3][0], ms[(i+2)%3][1]);
    render_ad(ctrls, tolerance, ms[(i+1)%3][0], ms[(i+1)%3][1],
                                ms[(i+2)%3][0], ms[(i+2)%3][1],
                                cs[i][0], cs[i][1]);

  } else if (ngood == 0) {
    for (int i = 0; i < 3; ++i) {
      render_ad(ctrls, tolerance, cs[i][0], cs[i][1],
                                  ms[(i+1)%3][0], ms[(i+1)%3][1],
                                  ms[(i+2)%3][0], ms[(i+2)%3][1]);
    }
    render_ad(ctrls, tolerance, ms[0][0], ms[0][1],
                                ms[1][0], ms[1][1],
                                ms[2][0], ms[2][1]);

  } else {
    glColor3f((float) rand() / RAND_MAX, (float) rand() / RAND_MAX, (float) rand() / RAND_MAX);
    glBegin(GL_TRIANGLES);
    send_gl(bezier_interp(ctrls, u0, v0));
    send_gl(bezier_interp(ctrls, u1, v1));
    send_gl(bezier_interp(ctrls, u2, v2));
    glEnd();
  }
}


void render_ad(const std::vector<vec3f> &patch, float tolerance) {
  render_ad(patch.data(), tolerance, 0,0, 1,0, 0,1);
  render_ad(patch.data(), tolerance, 1,0, 0,1, 1,1);
}


void render_object(const std::vector<std::vector<vec3f>> &patches,
                    bool adaptive, float parameter) {
  srand(0);
  for (const std::vector<vec3f> &patch : patches) {
    if (adaptive)
      render_ad(patch, parameter);
    else
      render_u(patch, parameter);
  }
}
