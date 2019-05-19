#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <iostream>
#include <random>

class Ball
{
public:
	bool init();
	void destroy();

	void start();

	int update(glm::vec2 lPaddle, glm::vec2 rPaddle);
	void draw();

	glm::vec2 getCurrentPosition();
private:
	GLuint VBO;
	GLuint VAO;
	GLuint EBO;

	glm::vec2 m_position;
	glm::vec3 translation;
	glm::mat4 model;

	std::vector<glm::vec2> generateQuad(glm::vec2 position, glm::vec2 size);
};