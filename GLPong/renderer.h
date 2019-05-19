#pragma once

#include <glad/glad.h>
#include <fstream>	//Really quick and dirty approach to load shaders in when initializing.
#include <iostream>
#include <sstream>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Renderer
{
public:
	bool init(GLADloadproc glfwLoadProc);	//This only initializes an opengl context from a pre-existing window, and loads basic blanking shaders for use.
	void destroy();

	void update();	//This sets the shader program, and sets its own uniforms.

	GLuint getProgramHandle();

private:
	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint program;

	glm::mat4 orthoMatrix;
};