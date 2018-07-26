#pragma once

#include "common.hpp"

struct b2Body;
struct b2Fixture;
struct b2FrictionJoint;

namespace shamblr {
namespace component {

struct Physics {
	glm::vec3 position;
	glm::vec3 velocity;
	glm::quat orientation;
	float mass;
	float size;
	b2Body* body;
	b2Fixture* fixture;
	b2FrictionJoint* joint;
};

struct Health {
	int current;
	int maximum;
};

struct Sprite {};

struct Player {};

struct Behaviour {
	enum class Type {
		NONE = 0,
		PLAYER,
		ZOMBIE,
	};
	Type type;
};

struct Sight {
	std::vector<Entity> entities;
};

struct Waypoint {
	glm::vec3 position;
};

} // namespace components
} // namespace shamblr
