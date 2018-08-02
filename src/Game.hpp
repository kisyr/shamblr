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
		void update(const Time&);

	private:
		Window m_window;
		std::shared_ptr<EntityRegistry> m_entities;
		std::shared_ptr<EventDispatcher> m_events;
		SystemManager m_systems;
		City m_city;
};

} // namespace shamblr
