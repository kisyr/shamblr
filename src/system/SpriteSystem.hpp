#pragma once

#include "System.hpp"
#include "../graphics.hpp"

namespace shamblr {

class SpriteSystem : public System {
	public:
		SpriteSystem();
		void configure();
		void enter(const Time&);
		void process(const Time&);
		void leave(const Time&);
		void constructSprite(EntityRegistry&, Entity);

	private:
		struct Vertex {
			glm::vec4 position;
			glm::vec2 texture;
		};
		struct Batch {
			GLuint texture;
			GLuint buffer;
			GLuint array;
			std::vector<Vertex> vertices;
		};

		std::unordered_map<std::string, Batch> m_batches;
		GLuint m_program;
};

} // namespace shamblr
