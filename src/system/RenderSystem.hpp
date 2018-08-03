#pragma once 

#include "System.hpp"
#include "../City.hpp"

namespace shamblr {

class RenderSystem : public System {
	public:
		RenderSystem(const City&);
		void enter(const Time&);
		void process(const Time&);
		void leave(const Time&);

	private:
		std::vector<glm::vec3> m_levelVertices;
};

} // namespace shamblr
