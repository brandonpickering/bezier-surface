#include <cstdio>
#include <iostream>
#include <string>
#include <utility>

#include <GLFW/glfw3.h>

#include "common.hpp"
#include "parse.hpp"


static std::vector<std::vector<vec3f>> object_patches;
static std::vector<std::pair<vec3f, float>> rotations;


static vec3f bezier_interp(const vec3f *ctrls, float u) {
  vec3f p1 = (1-u) * ctrls[0] + u * ctrls[1];
  vec3f p2 = (1-u) * ctrls[1] + u * ctrls[2];
  vec3f p3 = (1-u) * ctrls[2] + u * ctrls[3];
  vec3f p12 = (1-u) * p1 + u * p2;
  vec3f p23 = (1-u) * p2 + u * p3;
  return (1-u) * p12 + u * p23;
}

static vec3f bezier_interp(const vec3f *ctrls, float u, float v) {
  vec3f ctrls1[4];
  for (int i = 0; i < 4; ++i)
    ctrls1[i] = bezier_interp(ctrls + 4*i, u);
  return bezier_interp(ctrls1, v);
}


static vec3f send_gl(vec3f v) {
  glColor3f((float) rand()/RAND_MAX, (float) rand()/RAND_MAX, (float) rand()/RAND_MAX);
  glVertex3f(v.x, v.y, v.z);
}

static vec3f draw_patch(const std::vector<vec3f> &patch) {
  const vec3f *ctrls = patch.data();
  for (float u0 = 0; u0 < 1; u0 += 0.1f) {
    for (float v0 = 0; v0 < 1; v0 += 0.1f) {
      float u1 = std::min(u0 + 0.1f, 1.0f);
      float v1 = std::min(v0 + 0.1f, 1.0f);

      glBegin(GL_TRIANGLE_STRIP);
      send_gl(bezier_interp(ctrls, u0, v0));
      send_gl(bezier_interp(ctrls, u1, v0));
      send_gl(bezier_interp(ctrls, u0, v1));
      send_gl(bezier_interp(ctrls, u1, v1));
      glEnd();
    }
  }
}


static void render() {
  glLoadIdentity();
  for (int i = rotations.size() - 1; i >= 0; --i) {
    std::pair<vec3f, float> pair = rotations[i];
    glRotatef(pair.second, pair.first.x, pair.first.y, pair.first.z);
  }

  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  srand(0);
  for (const std::vector<vec3f> &patch : object_patches)
    draw_patch(patch);
}


static void load_object(std::string filename, std::istream &is) {
  object_patches = read_bezier(filename, is);
}


static void key_callback(GLFWwindow *window, int key, int scancode,
                          int action, int mods) {
  if (action == GLFW_RELEASE) return;

  const float rot_amt = 10;

  switch (key) {
    case GLFW_KEY_ESCAPE:
    case GLFW_KEY_Q:
      glfwSetWindowShouldClose(window, GLFW_TRUE);
      break;

    case GLFW_KEY_RIGHT:
      if (mods & GLFW_MOD_SHIFT) {
      } else {
        rotations.push_back({{0, 1, 0}, -rot_amt});
      }
      break;

    case GLFW_KEY_LEFT:
      if (mods & GLFW_MOD_SHIFT) {
      } else {
        rotations.push_back({{0, 1, 0}, rot_amt});
      }
      break;

    case GLFW_KEY_UP:
      if (mods & GLFW_MOD_SHIFT) {
      } else {
        rotations.push_back({{1, 0, 0}, rot_amt});
      }
      break;

    case GLFW_KEY_DOWN:
      if (mods & GLFW_MOD_SHIFT) {
      } else {
        rotations.push_back({{1, 0, 0}, -rot_amt});
      }
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
  glfwSwapInterval(0);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  double time = glfwGetTime();
  int frames = 0;

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

    ++frames;
    double elapsed = glfwGetTime() - time;
    if (elapsed >= 1) {
      std::string title = stringf("as3 - %d fps", (int) (frames / elapsed));
      glfwSetWindowTitle(window, title.c_str());
      frames = 0;
      time = glfwGetTime();
    }
  }

  return 0;
}
