#pragma once

#include "System.hpp"

namespace shamblr {

class HudSystem : public System {
	public:
		void process(const Time&);
};

} // namespace shamblr
