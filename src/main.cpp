#include <cstdio>
#include <iostream>
#include <string>

#include <GLFW/glfw3.h>

#include "common.hpp"
#include "parse.hpp"


static vec3f bezier_interp(vec3f *ctrls, float u) {
  vec3f p1 = (1-u) * ctrls[0] + u * ctrls[1];
  vec3f p2 = (1-u) * ctrls[1] + u * ctrls[2];
  vec3f p3 = (1-u) * ctrls[2] + u * ctrls[3];
  vec3f p12 = (1-u) * p1 + u * p2;
  vec3f p23 = (1-u) * p2 + u * p3;
  return (1-u) * p12 + u * p23;
}

static vec3f bezier_interp(vec3f *ctrls, float u, float v) {
  vec3f ctrls1[4];
  for (int i = 0; i < 4; ++i)
    ctrls1[i] = bezier_interp(ctrls + 4*i, u);
  return bezier_interp(ctrls1, v);
}


static void render() {
  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  vec3f ctrls[16] = {
    { -2, -3, 0 }, { -1, -1, 0 }, { 1, -1, 0 }, { 2, -3, 0 },
    { -2, -2, 0 }, { -1, 0, 0 }, { 1, 0, 0 }, { 2, -2, 0 },
    { -2, 0, 0 }, { -1, 1, 0 }, { 1, 1, 0 }, { 2, 0, 0 },
    { -2, 3, 0 }, { -1, 2, 0 }, { 1, 2, 0 }, { 2, 3, 0 },
  };

  for (float v = 0; v <= 1.1f; v += 0.1f) {
    glBegin(GL_LINE_STRIP);
    for (float u = 0; u <= 1.1f; u += 0.1f) {
      vec3f p = bezier_interp(&ctrls[0], u, v);
      glColor3f(v, u, 1-v);
      glVertex3f(p.x, p.y, p.z);
    }
    glEnd();
  }
}


static void load_object(std::string filename, std::istream &is) {
  std::vector<std::vector<vec3f>> patches = read_bezier(filename, is);
  for (std::vector<vec3f> &patch : patches) {
    for (size_t i = 0; i < 16; ++i) {
      printf("(%.2f %.2f %.2f) ", patch[i].x, patch[i].y, patch[i].z);
      if (i % 4 == 3) printf("\n");
    }
    printf("\n");
  }
}


static void key_callback(GLFWwindow *window, int key, int scancode,
                          int action, int mods) {
  if (action == GLFW_RELEASE) return;

  switch (key) {
    case GLFW_KEY_ESCAPE:
    case GLFW_KEY_Q:
      glfwSetWindowShouldClose(window, GLFW_TRUE);
      break;

    case GLFW_KEY_LEFT:
      if (mods & GLFW_MOD_SHIFT) {}
      break;

    default: break;
  }
}


int main(int argc, char *argv[]) {
  load_object("stdin", std::cin);

  glfwInit();

  GLFWwindow *window = glfwCreateWindow(400, 400, "as3", NULL, NULL);
  if (!window) {
    fprintf(stderr, "Failed to create window\n");
    glfwTerminate();
    return -1;
  }

  const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
  if (!mode) {
    fprintf(stderr, "Failed to get video mode\n");
    glfwTerminate();
    return -1;
  }

  glfwSetKeyCallback(window, key_callback);

  glfwMakeContextCurrent(window);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  while (!glfwWindowShouldClose(window)) {
    int win_width, win_height;
    glfwGetFramebufferSize(window, &win_width, &win_height);
    glViewport(0, 0, win_width, win_height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-3.5, 3.5, -3.5, 3.5, 5, -5);
    glMatrixMode(GL_MODELVIEW);

    render();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  return 0;
}
