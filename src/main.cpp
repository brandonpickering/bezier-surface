#include <cstdio>
#include <iostream>
#include <string>
#include <utility>

#include <GLFW/glfw3.h>

#include "common.hpp"
#include "parse.hpp"
#include "render.hpp"


static std::vector<std::vector<vec3f>> object_patches;

static std::vector<std::pair<vec3f, float>> rotations;
static vec3f translate;
static float zoom = 1;


static void render() {
  glLoadIdentity();
  glTranslatef(translate.x, translate.y, translate.z);
  for (int i = rotations.size() - 1; i >= 0; --i) {
    std::pair<vec3f, float> pair = rotations[i];
    glRotatef(pair.second, pair.first.x, pair.first.y, pair.first.z);
  }

  glClearColor(1, 1, 1, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  srand(0);
  render_object(object_patches, true, false, 0.1f);
}


static void load_object(std::string filename, std::istream &is) {
  object_patches = read_bezier(filename, is);
}


static void key_callback(GLFWwindow *window, int key, int scancode,
                          int action, int mods) {
  if (action == GLFW_RELEASE) return;

  const float rot_amt = 10;
  const float trans_amt = 0.3f;
  const float zoom_scale = 1.1f;

  switch (key) {
    case GLFW_KEY_ESCAPE:
    case GLFW_KEY_Q:
      glfwSetWindowShouldClose(window, GLFW_TRUE);
      break;

    case GLFW_KEY_RIGHT:
      if (mods & GLFW_MOD_SHIFT) {
        translate.x += trans_amt;
      } else {
        rotations.push_back({{0, 1, 0}, -rot_amt});
      }
      break;

    case GLFW_KEY_LEFT:
      if (mods & GLFW_MOD_SHIFT) {
        translate.x -= trans_amt;
      } else {
        rotations.push_back({{0, 1, 0}, rot_amt});
      }
      break;

    case GLFW_KEY_UP:
      if (mods & GLFW_MOD_SHIFT) {
        translate.y += trans_amt;
      } else {
        rotations.push_back({{1, 0, 0}, rot_amt});
      }
      break;

    case GLFW_KEY_DOWN:
      if (mods & GLFW_MOD_SHIFT) {
        translate.y -= trans_amt;
      } else {
        rotations.push_back({{1, 0, 0}, -rot_amt});
      }
      break;

    case GLFW_KEY_EQUAL:
      zoom *= zoom_scale;
      break;

    case GLFW_KEY_MINUS:
      zoom /= zoom_scale;
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
    float view = 3.5 / zoom;
    glOrtho(-view, view, -view, view, 5, -5);
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
