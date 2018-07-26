#pragma once

#include "System.hpp"
#include "../components.hpp"

namespace shamblr {

class CameraSystem : public System {
	public:
		void process(EntityRegistry&, const Time&);
};

} // namespace shamlr
