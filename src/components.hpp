#pragma once

#include "common.hpp"

namespace shamblr {

struct Position {
	glm::vec2 xy;
};

struct Velocity {
	glm::vec2 xy;
};

struct Health {
	int current;
	int maximum;
};

struct Sprite {};

} // namespace shamblr
