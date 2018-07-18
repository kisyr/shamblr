#include <iostream>
#include <GLFW/glfw3.h>

#include "Game.hpp"

int main() {
	if (!glfwInit()) {
		throw std::runtime_error("glfwInit");
	}

	GLFWwindow* window = glfwCreateWindow(1024, 768, "shamblr", NULL, NULL);
	if (!window) {
		throw std::runtime_error("glfwCreateWindow");
	}

	glfwMakeContextCurrent(window);

	shamblr::Game game;
	shamblr::Time time = {0};

	while (!glfwWindowShouldClose(window)) {
		const float delta = glfwGetTime();
		glfwSetTime(0);

		time.elapsed += delta;
		time.delta = delta;

		shamblr::Input input;
		for (int i = 0; i < input.keyboard.size(); ++i) {
			input.keyboard[i] = glfwGetKey(window, i);
		}
		glfwGetCursorPos(window, &input.mouse.x, &input.mouse.y);

		game.update(time, input);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return EXIT_SUCCESS;
}
