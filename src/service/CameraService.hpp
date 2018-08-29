#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace shamblr {

class CameraService {
	public:
		CameraService() : m_distance(10.0f) {}

		void centerOn(const glm::vec3& position) {
			m_target = position;
		}

		glm::vec3 unProject(const glm::vec3& point) const {
			return glm::unProject(point, view(), projection(), glm::vec4(m_viewport));
		}

		void viewport(const glm::ivec4& value) {
			m_viewport = value;
		}

		glm::mat4 projection() const {
			const auto vs = glm::vec2(viewport().z - viewport().x, viewport().w - viewport().y);
			return glm::perspective(50.0f, 1280.0f / 768.0f, 1.0f, 1000.0f);
		}

		glm::mat4 view() const {
			return glm::lookAt(
				m_target + glm::vec3(0.0f, m_distance, 0.0f),
				m_target,
				glm::vec3(0.0f, 0.0f, -1.0f)
			);
		}

		glm::ivec4 viewport() const {
			return m_viewport;
		}

		glm::vec3 target() const {
			return m_target;
		}

	private:
		glm::vec3 m_target;
		float m_distance;
		glm::ivec4 m_viewport;
};

} // namespace shamblr
