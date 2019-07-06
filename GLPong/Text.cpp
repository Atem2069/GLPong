#include "Text.h"

bool Text::init(std::string fontPath, int fontSize, int width, int height)
{
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "Error initializing freetype" << std::endl;
		return false;
	}

	FT_Face face;
	if (FT_New_Face(ft, fontPath.c_str(), 0, &face))
	{
		std::cout << "Error creating font face. The path given is likely invalid." << std::endl;
		return false;
	}

	FT_Set_Pixel_Sizes(face, 0,fontSize);


	//Loading all glyphs into map
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (int i = 0; i < 128; i++)
	{
		if (FT_Load_Char(face, i, FT_LOAD_RENDER))
		{
			std::cout << "Failed to load character.." << std::endl;
			continue;
		}

		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

		//Set up parameters for texture rendering.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		Character character =
		{
			texture,
			glm::ivec2(face->glyph->bitmap.width,face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left,face->glyph->bitmap_top),
			face->glyph->advance.x
		};

		characters.insert(std::pair<GLchar, Character>(i, character));
	}

	//Cleaning up remnants from truetype
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	//Loading Shaders
	const std::string vertexShaderSource =
	#include "Shaders\Text\vert.glsl"
		;

	const std::string fragmentShaderSource =
	#include "Shaders\Text\frag.glsl"
		;


	const char * vertShader = vertexShaderSource.c_str();
	const char * fragShader = fragmentShaderSource.c_str();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, &vertShader, 0);
	glShaderSource(fragmentShader, 1, &fragShader, 0);

	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);
	program = glCreateProgram();
	glAttachShader(program,vertexShader);
	glAttachShader(program,fragmentShader);

	glLinkProgram(program);

	proj = glm::ortho(0.0f,(float)width,0.0f,(float)height);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return true;
}

void Text::destroy()
{
	//todo.
}

void Text::drawText(std::string text, glm::vec2 position, glm::vec3 colour, float scale)
{
	glUseProgram(program);
	glUniform3f(1, colour.x, colour.y, colour.z);
	glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(proj));
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = characters[*c];

		GLfloat xpos = position.x + ch.bearing.x * scale;
		GLfloat ypos = position.y - (ch.size.y - ch.bearing.y) * scale;
		GLfloat w = ch.size.x * scale;
		GLfloat h = ch.size.y * scale;

		GLfloat vertices[6][4] = 
		{
			{xpos,ypos+h,0.0,0.0},	//Bottom left
			{xpos,ypos,0.0,1.0},	//Top left
			{xpos+w,ypos,1.0,1.0},	//Top right

			{xpos,ypos+h,0.0,0.0},
			{xpos+w,ypos,1.0,1.0},
			{xpos+w,ypos+h,1.0,0.0}
		};

		glBindTexture(GL_TEXTURE_2D, ch.textureID);

		glBindBuffer(GL_ARRAY_BUFFER,VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		position.x += (ch.advance >> 6) * scale;	//Scaling to pixels w bit shift 
	}
}