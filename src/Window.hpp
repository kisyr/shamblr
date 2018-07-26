#pragma once

#include <string>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace shamblr {

class Window {
	public:
		Window(const glm::ivec2& size, const std::string& name) : 
				m_size(size),
				m_name(name) {
			init();
			m_window = glfwCreateWindow(m_size.x, m_size.y, m_name.c_str(), NULL, NULL);
			if (!m_window) {
				throw std::runtime_error("glfwCreateWindow");
			}
		}

		void makeContextCurrent() {
			glfwMakeContextCurrent(m_window);
		}

		void swapBuffers() {
			glfwSwapBuffers(m_window);
		}

		bool shouldClose() const {
			return glfwWindowShouldClose(m_window);
		}

		double getTime() const {
			return glfwGetTime();
		}

		void setTime(const double time) {
			glfwSetTime(time);
		}

		glm::ivec2 size() const {
			return m_size;
		}

		std::string name() const {
			return m_name;
		}

		GLFWwindow* window() const {
			return m_window;
		}

		static void init() {
			static bool initialized = false;
			if (!initialized) {
				if (!glfwInit()) {
					throw std::runtime_error("glfwInit");
				}
				initialized = true;
			}
		}

		static void pollEvents() {
			glfwPollEvents();
		}

	private:
		GLFWwindow* m_window;
		glm::ivec2 m_size;
		std::string m_name;
};

} // namespace shamblr
