#pragma once 

#include "System.hpp"
#include "../components.hpp"

namespace shamblr {

class RenderSystem : public System {
	public:
		void enter(const Time&);
		void process(EntityRegistry&, const Time&);
		void leave(const Time&);

	private:
		struct Vertex {
			glm::vec3 position;
			glm::vec4 color;
		};
		std::vector<Vertex> m_vertices;
};

} // namespace shamblr
