#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

#include "parse.hpp"


std::vector<std::vector<vec3f>> read_bezier(std::string filename,
                                              std::istream &is) {
  size_t num_patches;
  if (!(is >> num_patches)) {
    fprintf(stderr, "%s: missing number of patches\n", filename.c_str());
    return std::vector<std::vector<vec3f>>();
  }

  std::vector<std::vector<vec3f>> patches(num_patches, std::vector<vec3f>(16));

  for (std::vector<vec3f> &patch : patches) {
    for (vec3f &ctrl : patch) {
      for (int i = 0; i < 3; ++i) {
        if (!(is >> ctrl.data[i])) {
          fprintf(stderr, "%s: not enough control point data\n", 
                              filename.c_str());
          return patches;
        }
      }
    }
  }

  return patches;
}
