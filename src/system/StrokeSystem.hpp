#pragma once

#include "System.hpp"
#include "../graphics.hpp"

namespace shamblr {

class StrokeSystem : public System {
	public:
		enum { MaxStrokes = 1000 };

		StrokeSystem(std::shared_ptr<EntityRegistry>, std::shared_ptr<EventDispatcher>);
		void enter(const Time&);
		void process(const Time&);
		void leave(const Time&);

	private:
		struct Vertex {
			glm::vec4 position;
		};

		std::vector<Vertex> m_vertices;
		GLuint m_buffer;
		GLuint m_array;
		GLuint m_program;
};

} // namespace shamblr
