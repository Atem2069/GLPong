#include "paddle.h"

bool Paddle::init(glm::vec2 position, glm::vec2 size)
{
	m_position = position;

	std::vector<glm::vec2> quad = generateQuad(position, size);
	unsigned int indices[6] = { 0,1,2,2,3,0 };	//Indices for CCW quad, which conforms with default GL system when culling back-side.

	//Generate VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, quad.size() * sizeof(glm::vec2), &quad[0], GL_STATIC_DRAW);

	//Generate EBO
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	//Generate VAO and set attrib call
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	model = glm::mat4(1.0);
	return true;
}

void Paddle::destroy()
{
	///TODO: Complete. cba
}

void Paddle::update(float yVelocity)
{
	if ((m_position.y + 50 >= 600 && yVelocity > 0) || (m_position.y - 50 <= 0 && yVelocity < 0))
		yVelocity = 0;
	m_position.y += yVelocity;
	model = glm::translate(model, glm::vec3(0, yVelocity, 0));
	glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(model));
}

void Paddle::draw()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

glm::vec2 Paddle::getCurrentPosition()
{
	return m_position;
}

std::vector<glm::vec2> Paddle::generateQuad(glm::vec2 position, glm::vec2 size)
{
	std::vector<glm::vec2> quad;

	quad.push_back(glm::vec2(position.x + (size.x / 2), position.y - (size.y / 2)));	//Bottom right
	quad.push_back(glm::vec2(position.x + (size.x / 2), position.y + (size.y / 2)));	//Top right
	quad.push_back(glm::vec2(position.x - (size.x / 2), position.y + (size.y / 2)));	//Top left
	quad.push_back(glm::vec2(position.x - (size.x / 2), position.y - (size.y / 2)));	//Bottom left
	return quad;
}