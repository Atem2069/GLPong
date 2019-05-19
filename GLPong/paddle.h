#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <iostream>

class Paddle
{
public:
	bool init(glm::vec2 position,glm::vec2 size);
	void destroy();

	void reset();

	void update(float yVelocity);
	void draw();

	glm::vec2 getCurrentPosition();

private:
	GLuint VBO;
	GLuint VAO;
	GLuint EBO;

	glm::vec2 m_position;
	glm::mat4 model;

	std::vector<glm::vec2> generateQuad(glm::vec2 position, glm::vec2 size);
};