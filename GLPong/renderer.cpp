#include "renderer.h"

bool Renderer::init(GLADloadproc glfwLoadProc)
{
	if (!gladLoadGLLoader(glfwLoadProc))	
	{
		std::cout << "Failed to load OpenGL functions.." << std::endl;
		return false;
	}

	const std::string vertexShaderSource =			
	#include "shaders\pongVtx.glsl"
	;
	const char * vertSrc = vertexShaderSource.c_str();

	const std::string fragmentShaderSource =
	#include "Shaders\pongFrag.glsl"
	;
	const char * fragSrc = fragmentShaderSource.c_str();

	GLint compileStatus = 0;

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertSrc, nullptr);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus == GL_FALSE)
	{
		std::cout << "Failed to compile vertex shader." << std::endl;
		return false;
	}

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragSrc, nullptr);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus == GL_FALSE)
	{
		std::cout << "Failed to compile fragment shader." << std::endl;
		return false;
	}
	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &compileStatus);
	if (compileStatus == GL_FALSE)
	{
		std::cout << "Failed to link shader program." << std::endl;
		return false;
	}

	glUseProgram(program);

	orthoMatrix = glm::ortho(0.0f, 600.0f, 0.0f, 600.0f, 0.0f, 1000.0f);
	return true;
}

void Renderer::destroy()
{
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteProgram(program);
}

void Renderer::update()
{
	glUseProgram(program);
	glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(orthoMatrix));
}

GLuint Renderer::getProgramHandle()
{
	return program;
}