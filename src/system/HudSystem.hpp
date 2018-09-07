#pragma once

#include "System.hpp"
#include "../graphics.hpp"

namespace shamblr {

class HudSystem : public System {
	public:
		HudSystem(std::shared_ptr<EntityRegistry>, std::shared_ptr<EventDispatcher> events);
		void enter(const Time&);
		void process(const Time&);
		void leave(const Time&);

	private:
		NVGcontext* m_context;
};

} // namespace shamblr
