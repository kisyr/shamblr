#pragma once

#include "System.hpp"

namespace shamblr {

class HudSystem : public System {
	public:
		HudSystem(std::shared_ptr<EntityRegistry>, std::shared_ptr<EventDispatcher> events);
		void process(const Time&);
};

} // namespace shamblr
