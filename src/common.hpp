#pragma once

#include <array>
#include <glm/glm.hpp>
#include <entt/entt.hpp>

namespace shamblr {

typedef entt::DefaultRegistry EntityRegistry;
typedef EntityRegistry::entity_type Entity;

struct Time {
	float elapsed;
	float delta;
};

struct Input {
	std::array<int, 256> keyboard;
	glm::dvec2 mouse;
};

} // namespace shamblr
