#pragma once

#include "common.hpp"
#include "system/System.hpp"
#include "City.hpp"

namespace shamblr {

class Game {
	public:
		Game(const glm::ivec2&);
		void configure();
		void run();

	private:
		City m_city;
		std::shared_ptr<EntityRegistry> m_entities;
		std::shared_ptr<EventDispatcher> m_events;
		std::shared_ptr<SystemManager> m_systems;
};

} // namespace shamblr
