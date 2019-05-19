#include "ball.h"

bool Ball::init()
{
	m_position = glm::vec2(300, 300);	//Starting position for the ball. centre of screen works nicely.
	std::vector<glm::vec2> quad = generateQuad(m_position, glm::vec2(15, 15));
	unsigned int indices[6] = { 0,1,2,2,3,0 };

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, quad.size() * sizeof(glm::vec2), &quad[0], GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	model = glm::mat4(1.0f);
	return true;
}

void Ball::destroy()
{
	///TODO
}

void Ball::start()
{
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> dist(0, 360);	//Random starting angle between -90 degrees and 90.
	float randomAngle = dist(rng);
	m_position = glm::vec2(300, 300);
	model = glm::mat4(1.0f);
	translation = glm::vec3(glm::cos(glm::radians(randomAngle)), -glm::sin(glm::radians(randomAngle)), 0.0f) * 7.5f;
}

int Ball::update(glm::vec2 lPaddle, glm::vec2 rPaddle)
{
	model = glm::translate(model, translation);
	m_position.x += translation.x;
	m_position.y += translation.y;

	//Generate random vector so that paddle bouncing has variation
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> dist(-90,90);	//More variation for paddle hits.
	float randInt = dist(rng);
	glm::vec2 variationTransform = glm::vec2(glm::cos(glm::radians(randInt)), -glm::sin(glm::radians(randInt)));


	float ballFarRight = m_position.x + 7.5f;
	float ballFarLeft = m_position.x - 7.5f;
	float ballTop = m_position.y + 7.5f;
	float ballBottom = m_position.y - 7.5f;
	if (ballFarRight >= rPaddle.x - 5 && ballFarRight < rPaddle.x + 5  && ballBottom <= rPaddle.y + 50 && ballTop >= rPaddle.y - 50 && translation.x > 0)
	{
		glm::vec2 vec2trans = glm::vec2(translation.x, translation.y);
		vec2trans = vec2trans - 2.0f * glm::vec2(-1, 0) * glm::dot(glm::vec2(-1, 0), vec2trans);
		vec2trans += variationTransform;
		translation = glm::vec3(vec2trans, 0.0f);
	}
	if (ballFarLeft <= lPaddle.x + 5 && ballFarLeft > lPaddle.x - 5 && ballBottom <= lPaddle.y + 50 && ballTop >= lPaddle.y - 50 && translation.x < 0)
	{
		glm::vec2 vec2trans = glm::vec2(translation.x, translation.y);
		vec2trans = vec2trans - 2.0f * glm::vec2(1, 0) * glm::dot(glm::vec2(1, 0), vec2trans);
		vec2trans += variationTransform;
		translation = glm::vec3(vec2trans, 0.0f);
	}
	if (ballTop >= 600 && translation.y > 0)
	{
		glm::vec2 vec2trans = glm::vec2(translation.x, translation.y);
		vec2trans = vec2trans - 2.0f * glm::vec2(0, -1) * glm::dot(glm::vec2(0, -1), vec2trans);
		translation = glm::vec3(vec2trans, 0.0f);
	}
	if (ballBottom <= 0 && translation.y < 0)
	{
		glm::vec2 vec2trans = glm::vec2(translation.x, translation.y);
		vec2trans = vec2trans - 2.0f * glm::vec2(0, 1) * glm::dot(glm::vec2(0, 1), vec2trans);
		translation = glm::vec3(vec2trans, 0.0f);
	}

	int retCode = 0;

	if (ballFarRight > 600)
		retCode = 1;
	else if (ballFarLeft < 0)
		retCode = 2;
	glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(model));

	return retCode;
}

void Ball::draw()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

glm::vec2 Ball::getCurrentPosition()
{
	return m_position;
}

std::vector<glm::vec2> Ball::generateQuad(glm::vec2 position, glm::vec2 size)
{
	std::vector<glm::vec2> quad;

	quad.push_back(glm::vec2(position.x + (size.x / 2), position.y - (size.y / 2)));	//Bottom right
	quad.push_back(glm::vec2(position.x + (size.x / 2), position.y + (size.y / 2)));	//Top right
	quad.push_back(glm::vec2(position.x - (size.x / 2), position.y + (size.y / 2)));	//Top left
	quad.push_back(glm::vec2(position.x - (size.x / 2), position.y - (size.y / 2)));	//Bottom left
	return quad;
}