#include "renderer.h"
#include "paddle.h"
#include "ball.h"
#include "Text.h"
#include <GLFW/glfw3.h>
#include <iostream>

#include <vector>
#include <glm/glm.hpp>


int main()
{
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(600, 600, "GLPong", nullptr, nullptr);
	if (!window)
		return -1;
	
	glfwMakeContextCurrent(window);

	std::cout << "Initializing renderer" << std::endl;
	Renderer renderer;
	if (!renderer.init((GLADloadproc)glfwGetProcAddress))
		return -1;

	Ball ball;
	ball.init();

	Paddle lPaddle;
	lPaddle.init(glm::vec2(50, 300), glm::vec2(10, 100));

	Paddle rPaddle;
	rPaddle.init(glm::vec2(550, 300), glm::vec2(10, 100));

	Text sampleText;
	if (!sampleText.init("Fonts\\bit5x5.ttf", 96, 600, 600))
		return -1;

	glfwSwapInterval(1);

	int lYVelocity = 0, rYVelocity = 0;
	int roundRet = 0;

	bool rKeyDepressed = false;

	std::cout << "All GL objects passed." << std::endl;

	int fps = 0;
	int lscore = 0, rscore = 0;
	while (glfwGetKey(window, GLFW_KEY_ENTER) != GLFW_PRESS && !glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);
		sampleText.drawText("PONG", glm::vec2(150, 500), glm::vec3(1, 0, 0), 1.0f);
		sampleText.drawText("Press ENTER to play", glm::vec2(150, 400), glm::vec3(1, 1, 1), 0.25f);
		glfwSwapBuffers(window);
	}


	float oldTime = glfwGetTime(), newTime, deltaTime = 0;
	ball.start();
	lPaddle.reset();
	rPaddle.reset();
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		lYVelocity = 0;
		rYVelocity = 0;

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			rYVelocity = 10;
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			rYVelocity = -10;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			lYVelocity = 10;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			lYVelocity = -10;
		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && rKeyDepressed == false)
		{
			rKeyDepressed = true;
			std::cout << "Round reset" << std::endl;
			ball.start();
			lPaddle.reset();
			rPaddle.reset();
			lscore = 0;
			rscore = 0;
		}
		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE)
			rKeyDepressed = false;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderer.update();

		lPaddle.update(lYVelocity,deltaTime);
		lPaddle.draw();

		rPaddle.update(rYVelocity,deltaTime);
		rPaddle.draw();

		roundRet = ball.update(lPaddle.getCurrentPosition(),rPaddle.getCurrentPosition(),deltaTime);
		ball.draw();

		sampleText.drawText(std::to_string(lscore), glm::vec2(100, 500), glm::vec3(1, 1, 1), 1.0f);
		sampleText.drawText(std::to_string(rscore), glm::vec2(450, 500), glm::vec3(1, 1, 1), 1.0f);
		sampleText.drawText(std::to_string(fps)+" FPS", glm::vec2(0, 10), glm::vec3(1, 1, 1), 0.25f);
		glfwSwapBuffers(window);

		if (roundRet)
		{
			if (roundRet == 1)
			{
				std::cout << "Round has ended. Player 1 wins" << std::endl;
				lscore += 1;
			}
			else if (roundRet == 2)
			{
				std::cout << "Round has ended. Player 2 wins" << std::endl;
				rscore += 1;
			}

			ball.start();	//Reset ball position.
			lPaddle.reset();
			rPaddle.reset();
		}

		newTime = glfwGetTime();
		deltaTime = newTime - oldTime;
		oldTime = newTime;
		fps = 1 / deltaTime;
	}

	return 0;
}