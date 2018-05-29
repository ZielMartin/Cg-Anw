#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/mat3x3.hpp>


//A basic class for handling OpenGL (GLSL) shaders
class Shader {
public:
	Shader();
	//@vsFile The path to the vertex shader text file
	//@fsFile The path to the fragment shader text file
	Shader(const char *vsFile, const char *fsFile);
	~Shader();

	//Load shader text files and create the OpenGL program for them
	void Init(const char *vsFile, const char *fsFile);

	//Tell OpenGL to use this shader, i.e. "Switch on" the shader
	void Bind();
	//"Switch off" shader
	void Unbind();
	//The ID used by OpenGL to recognise the shader program.  Used mainly to find uniform variable locations in the program
	unsigned int ID();

	void passUniformToShader(glm::mat4 &modelMatrix,
							 glm::mat4 &viewMatrix,
							 glm::mat4 &projectionMatrix,
							 glm::mat3 &normalMatrix,
							 glm::vec4 &viewPort);


	private:
	//Utility function to load in a text file
	const char *LoadTextFile(const char *filename);

	//The ID for the shader program.  Used by OpenGL to identify the appropriate shader program.
	unsigned int m_shaderID;
	//The ID for the vertex shader
	unsigned int m_shaderVertexProgram;

	//The ID for the fragment shader
	unsigned int m_shaderFragmentProgram;
};
