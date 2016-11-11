#ifndef _RENDER_HPP
#define _RENDER_HPP


#include <vector>

#include "common.hpp"


void render_u(const std::vector<vec3f> &patch, bool wireframe, float step);
void render_ad(const std::vector<vec3f> &patch, bool wireframe,
                float tolerance);

void render_object(const std::vector<std::vector<vec3f>> &patches,
                    bool adaptive, bool wireframe, float parameter);

#endif
