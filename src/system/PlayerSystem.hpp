#pragma once

#include "System.hpp"
#include "../common.hpp"
#include "../components.hpp"

namespace shamblr {

class PlayerSystem : public System {
	public:
		PlayerSystem(std::shared_ptr<EntityRegistry>, std::shared_ptr<EventDispatcher>);
		void process(const Time&);
};

} // namespace shamblr
