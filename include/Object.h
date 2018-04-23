#pragma once

#ifndef OBJECT_H
#define OBJECT_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

// Converts degrees to radians.
#define degreesToRadians(angleDegrees) (angleDegrees * M_PI / 180.0)

// Converts radians to degrees.
#define radiansToDegrees(angleRadians) (angleRadians * 180.0 / M_PI)

namespace cg {
struct Object {
	GLuint vao = 0;
	GLuint positionBuffer;
	GLuint colorBuffer;
	GLuint normalBuffer;
	GLuint indexBuffer;
	GLuint indexCount;

    bool condition = true;
    GLenum mode = GL_LINES;

	~Object() {
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &indexBuffer);
		glDeleteBuffers(1, &normalBuffer);
		glDeleteBuffers(1, &colorBuffer);
		glDeleteBuffers(1, &positionBuffer);
	}
};

}

#endif
