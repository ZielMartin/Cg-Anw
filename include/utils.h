#ifndef _utils_H
#define _utils_H

#include <iostream>
#include <vector>
#include "glm/glm.hpp"

/** Return the maximum of three floats **/
float fmax(float f1, float f2, float f3);

/** Return the minimum of three floats **/
float fmin(float f1, float f2, float f3);

/** Convert a vec3 to an equivalent vector of doubles **/
std::vector<double> to_vector(glm::vec3 vec);

/** Convert a vector of doubles back to an equivalent vec3 **/
glm::vec3 to_vec3(std::vector<double> vec);

/** Allows for vec3 objects to be printed to streams **/
std::ostream & operator << (std::ostream & stream, const glm::vec3 & obj);

/** Allows for vec2 objects to be printed to streams **/
std::ostream & operator << (std::ostream & stream, const glm::vec2 & obj);

#endif
