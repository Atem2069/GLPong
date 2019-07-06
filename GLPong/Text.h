#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include <glad/glad.h>

#include <iostream>
#include <string>
#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Character
{
	GLuint textureID;
	glm::ivec2 size;
	glm::ivec2 bearing;
	GLuint advance;
};

class Text
{
public:
	bool init(std::string fontPath, int width, int height);
	void destroy();

	void drawText(std::string text, glm::vec2 position, glm::vec3 colour, float scale);

private:
	std::map<GLchar, Character> characters;
	GLuint VAO, VBO;
	GLuint program;

	glm::mat4 proj;
};