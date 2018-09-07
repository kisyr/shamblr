#include "HudSystem.hpp"
#include "../components.hpp"
#include "../service/GraphicsService.hpp"

using namespace shamblr;

HudSystem::HudSystem(
	std::shared_ptr<EntityRegistry> entities,
	std::shared_ptr<EventDispatcher> events
) : System(entities, events) {}

void HudSystem::process(const Time& time) {}
