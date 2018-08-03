#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace shamblr {

class WindowService {
	public:
		WindowService(const glm::ivec2& size) : m_size(size) {
			static int instances = 0;
			if (instances > 0) {
				throw std::runtime_error("WindowService::instances > 0");
			}
			if (!glfwInit()) {
				throw std::runtime_error("glfwInit");
			}
			m_window = glfwCreateWindow(size.x, size.y, "Shamblr", NULL, NULL);
			if (!m_window) {
				throw std::runtime_error("glfwCreateWindow");
			}
			glfwMakeContextCurrent(m_window);
			if (glewInit() != GLEW_OK) {
				throw std::runtime_error("glewInit");
			}
		}

		void refresh() {
			glfwSwapBuffers(m_window);
			glfwPollEvents();
		}

		bool isOpen() const {
			return !glfwWindowShouldClose(m_window);
		}

		int getKey(int key) const {
			return glfwGetKey(m_window, key);
		}

		int getMouseButton(int button) const {
			return glfwGetMouseButton(m_window, button);
		}

		glm::vec2 getMouseCursor() const {
			glm::dvec2 cursor;
			glfwGetCursorPos(m_window, &cursor.x, &cursor.y);
			return glm::vec2(cursor);
		}

		glm::ivec2 getSize() const {
			return m_size;
		}

	private:
		GLFWwindow* m_window;
		glm::ivec2 m_size;
};

} // namespace shamblr
