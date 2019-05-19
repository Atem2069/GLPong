#include "renderer.h"
#include "paddle.h"
#include "ball.h"
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

	Renderer renderer;
	if (!renderer.init((GLADloadproc)glfwGetProcAddress))
		return -1;

	Ball ball;
	ball.init();
	ball.start();

	Paddle lPaddle;
	lPaddle.init(glm::vec2(50, 300), glm::vec2(10, 100));

	Paddle rPaddle;
	rPaddle.init(glm::vec2(550, 300), glm::vec2(10, 100));

	glfwSwapInterval(1);

	int lYVelocity = 0, rYVelocity = 0;
	int roundRet = 0;
	while (!glfwWindowShouldClose(window))
	{
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
		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		{
			std::cout << "Round reset" << std::endl;
			ball.start();
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderer.update();

		lPaddle.update(lYVelocity);
		lPaddle.draw();

		rPaddle.update(rYVelocity);
		rPaddle.draw();

		roundRet = ball.update(lPaddle.getCurrentPosition(),rPaddle.getCurrentPosition());
		ball.draw();

		glfwSwapBuffers(window);
		glfwPollEvents();

		if (roundRet)
		{
			if (roundRet == 1)
			{
				std::cout << "Round has ended. Player 1 wins" << std::endl;
			}
			else if (roundRet == 2)
			{
				std::cout << "Round has ended. Player 2 wins" << std::endl;
			}

			ball.start();	//Reset ball position.
		}
	}

	return 0;
}