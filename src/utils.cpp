#include "utils.h"

float fmax(float f1,float f2, float f3) {
	float f = f1;
	if (f < f2) f = f2;
	if (f < f3) f = f3;
	return f;
}

float fmin(float f1,float f2, float f3) {
	float f = f1;
	if (f > f2) f = f2;
	if (f > f3) f = f3;
	return f;
}

std::vector<double> to_vector(glm::vec3 v_vec3) {
    std::vector<double> v_vector;
    v_vector.push_back(v_vec3.x);
    v_vector.push_back(v_vec3.y);
    v_vector.push_back(v_vec3.z);
    return v_vector;
}

glm::vec3 to_vec3(std::vector<double> v_vector) {
    return glm::vec3(v_vector[0], v_vector[1], v_vector[2]);
}

std::ostream & operator << (std::ostream & stream, const glm::vec3 & obj) {
	stream << obj.x << ' ' << obj.y << ' ' << obj.z << ' ';
	return stream;
}

std::ostream & operator << (std::ostream & stream, const glm::vec2 & obj) {
	stream << obj.x << ' ' << obj.y << ' ';
	return stream;
}
