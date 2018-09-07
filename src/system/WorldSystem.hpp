#pragma once 

#include "System.hpp"
#include "../graphics.hpp"
#include "../City.hpp"

namespace shamblr {

class WorldSystem : public System {
	public:
		WorldSystem(std::shared_ptr<EntityRegistry>, std::shared_ptr<EventDispatcher>, const City&);
		void enter(const Time&);
		void process(const Time&);
		void leave(const Time&);

	private:
		std::vector<glm::vec3> m_levelVertices;
		GLuint m_levelBuffer;
		GLuint m_levelArray;
		GLuint m_levelProgram;
};

} // namespace shamblr
