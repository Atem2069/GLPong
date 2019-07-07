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
	GLuint textureID;	//GL texture handle
	glm::ivec2 size;	//Actual size of the texture
	glm::ivec2 bearing;	//Bearing being positioning into actual character from origin
	GLuint advance;	//Spacing to next char
};

class Text
{
public:
	bool init(std::string fontPath, int fontSize, int width, int height);
	void destroy();

	void drawText(std::string text, glm::vec2 position, glm::vec3 colour, float scale);

private:
	std::map<GLchar, Character> characters;
	GLuint VAO, VBO;
	GLuint program;

	glm::mat4 proj;
};