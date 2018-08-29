#pragma once

#include "System.hpp"
#include "../graphics.hpp"

namespace shamblr {

class SpriteRenderSystem : public System {
	public:
		SpriteRenderSystem();
		void enter(const Time&);
		void process(const Time&);
		void leave(const Time&);

	private:
		struct Vertex {
			glm::vec4 position;
			glm::vec2 texture;
		};
		struct Batch {
			GLuint texture;
			GLuint buffer;
			std::vector<Vertex> vertices;
		};

		std::unordered_map<std::string, Batch> m_batches;
		GLuint m_program;
};

} // namespace shamblr
