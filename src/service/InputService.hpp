#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace shamblr {

class InputService {
	public:
		InputService(GLFWwindow* window) : m_window(window) {}

		int keyState(int key) {
			return glfwGetKey(m_window, key);
		}

		glm::vec2 cursor() {
			glm::dvec2 xy;
			glfwGetCursorPos(m_window, &xy.x, &xy.y);
			return glm::vec2(xy);
		}

	private:
		GLFWwindow* m_window;
};

} // namespace shamblr
