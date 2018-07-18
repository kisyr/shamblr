#pragma once

#include "common.hpp"
#include "components.hpp"
#include "System.hpp"

namespace shamblr {

class RenderSystem : public System {
	public:
		void update(EntityRegistry& registry, const Time& time) {
			std::vector<Vertex> vertices;
			registry.view<Position, Sprite>().each([&vertices](auto entity, auto& position, auto& sprite) {
				const float sh = 0.5f;
				const glm::vec4 color = glm::vec4(1,0,0,1);
				vertices.push_back(Vertex{position.xy + glm::vec2(-sh, -sh), color});
				vertices.push_back(Vertex{position.xy + glm::vec2(-sh, +sh), color});
				vertices.push_back(Vertex{position.xy + glm::vec2(+sh, -sh), color});
				vertices.push_back(Vertex{position.xy + glm::vec2(-sh, +sh), color});
				vertices.push_back(Vertex{position.xy + glm::vec2(+sh, +sh), color});
				vertices.push_back(Vertex{position.xy + glm::vec2(+sh, -sh), color});
			});
			glBegin(GL_TRIANGLES);
			for (auto& v : vertices) {
				glVertex2f(v.position.x, v.position.y);
			}
			glEnd();
		}

	private:
		struct Vertex {
			glm::vec2 position;
			glm::vec4 color;
		};
};

} // namespace shamblr
