#pragma once

#include "common.hpp"

namespace shamblr {
namespace events {

struct Attack {
	Entity origin;
	Entity target;
};

struct Damage {
	Entity victim;
	int amount;
};

struct ProjectileHit {
	Entity victim;
};

struct RayCast {
	glm::vec3 origin;
	glm::vec3 direction;
	float length;
	std::vector<Entity> entities;
};

} // namespace events
} // namespace shamblr
