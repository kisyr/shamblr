#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace shamblr {

class InputService {
	public:
		InputService(GLFWwindow* window) : m_window(window) {}

		int getKey(int key) const {
			return glfwGetKey(m_window, key);
		}

		int getMouseButton(int button) const {
			return glfwGetMouseButton(m_window, button);
		}

		glm::vec2 getMouseCursor() const {
			glm::dvec2 xy;
			glfwGetCursorPos(m_window, &xy.x, &xy.y);
			return glm::vec2(xy);
		}

	private:
		GLFWwindow* m_window;
};

} // namespace shamblr
