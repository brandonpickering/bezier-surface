#ifndef _PARSE_HPP
#define _PARSE_HPP


#include <string>
#include <vector>

#include "common.hpp"


std::vector<std::vector<vec3f>> read_bezier(std::string filename,
                                              std::istream &is);


#endif
