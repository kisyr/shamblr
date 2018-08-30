#pragma once

#include "common.hpp"

struct b2Body;
struct b2Fixture;
struct b2FrictionJoint;
#if 0
struct Weapon {
	float caliber;
	float velocity;
	float rate;
	float fired;
	bool triggered;
	int magazine;
};
#endif
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

struct Ray {
	glm::vec3 origin;
	glm::vec3 direction;
};

struct Health {
	int current;
	int maximum;
	float trauma;
};

struct Sprite {
	std::string description;
	std::string batch;
	std::vector<glm::vec4> frames;
	std::unordered_map<std::string, std::vector<size_t>> cycles;
	std::string cycle;
	size_t cycleIndex;
	float elapsed;
};

struct Tracer {
	glm::vec3 begin;
	glm::vec3 end;
};

struct Player {
	std::pair<glm::vec3, glm::vec3> aim;
};

struct Behaviour {
	enum class Type {
		NONE = 0,
		PLAYER,
		ZOMBIE,
	};
	Type type;
};

struct Sight {
	float distance;
	std::vector<Entity> entities;
};

struct Waypoint {
	glm::vec3 position;
};

struct Item {
	Entity owner;
	bool used;
};

struct Weapon {
	float velocity;
	int rate;
	float fired;
	glm::vec3 origin;
	glm::vec3 direction;
};

struct Inventory {
	std::vector<Entity> items;
};

struct Projectile {
	float velocity;
};

} // namespace components
} // namespace shamblr
