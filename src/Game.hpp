#pragma once

#include "common.hpp"
#include "Window.hpp"
#include "City.hpp"
#include "system/System.hpp"

namespace shamblr {

class Game {
	public:
		Game();
		void configure();
		void run();

	private:
		Window m_window;
		City m_city;
		std::shared_ptr<EntityRegistry> m_entities;
		std::shared_ptr<EventDispatcher> m_events;
		std::shared_ptr<SystemManager> m_systems;
};

} // namespace shamblr
