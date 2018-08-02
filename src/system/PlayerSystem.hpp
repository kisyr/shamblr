#pragma once

#include "System.hpp"
#include "../common.hpp"
#include "../components.hpp"

namespace shamblr {

class PlayerSystem : public System {
	public:
		void process(const Time&);
};

} // namespace shamblr
