#pragma once 

#include "System.hpp"
#include "../components.hpp"
#include "../events.hpp"
#include "../util.hpp"

namespace shamblr {

class BehaviourSystem : public System {
	public:
		void process(const Time& time) {
			// Map behaviour
			entities()->view<component::Behaviour>().each(
				[this, &time](const auto entity, auto& behaviour) {
					switch (behaviour.type) {
						case component::Behaviour::Type::ZOMBIE:
							this->behaveZombie(entity, time);
							break;
					}
				}
			);
		}

		void behaveZombie(const Entity entity, const Time& time) {
			auto& physics = entities()->get<component::Physics>(entity);
			auto& sight = entities()->get<component::Sight>(entity);

			// Focus on player sightings
			std::vector<Entity> players;
			std::copy_if(sight.entities.begin(), sight.entities.end(), std::back_inserter(players), 
				[this](const auto sightedEntity) {
					if (this->entities()->has<component::Behaviour>(sightedEntity)) {
						const auto& behaviour = this->entities()->get<component::Behaviour>(sightedEntity);
						if (behaviour.type == component::Behaviour::Type::PLAYER) {
							return true;
						}
					}
					return false;
				}
			);

			// Manage a waypoint to sighted entity
			if (!players.empty()) {
				const auto target = players.back();
				const auto& targetPhysics = entities()->get<component::Physics>(target);
				entities()->accommodate<component::Waypoint>(entity, targetPhysics.position);
			}

			// Move towards waypoint
			// TODO: Should probably be moved to generalized section
			if (entities()->has<component::Waypoint>(entity)) {
				const auto& waypoint = entities()->get<component::Waypoint>(entity);
				const auto direction = glm::normalize(waypoint.position - physics.position);
				const auto angle = glm::atan(direction.x, direction.z);
				const auto orientation = glm::quat(glm::vec3(0.0f, angle, 0.0f));
				const float maxAngle = glm::pi<float>() * 0.25f * time.delta;
				const auto deltaAngle = util::angleBetween(physics.orientation, orientation);
				const auto movementSpeed = glm::max(1.0f - (deltaAngle / glm::half_pi<float>()), 0.3f);
				physics.orientation = util::rotateTowards(physics.orientation, orientation, maxAngle);
				physics.velocity += (physics.orientation * glm::vec3(0.0f, 0.0f, 1.0f)) * movementSpeed;
			}

			// Attack target
			if (!players.empty()) {
				const auto target = players.back();
				const auto& targetPhysics = entities()->get<component::Physics>(target);
				if (glm::distance(targetPhysics.position, physics.position) < 2.0f) {
					//this->events()->enqueue<events::Damage>(target, 10);
				}
			}
		}
};

} // namespace shamblr
