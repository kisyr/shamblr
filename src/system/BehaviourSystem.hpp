#pragma once 

#include "System.hpp"
#include "../components.hpp"
#include "../events.hpp"

namespace shamblr {

class BehaviourSystem : public System {
	public:
		void process(const Time& time) {
			// Map behaviour
			entities()->view<component::Behaviour>().each(
				[this](const auto entity, auto& behaviour) {
					switch (behaviour.type) {
						case component::Behaviour::Type::ZOMBIE:
							this->behaveZombie(entity);
							break;
					}
				}
			);
		}

		void behaveZombie(const Entity entity) {
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
				physics.velocity = direction * 2.0f;
			}

			// Attack target
			if (!players.empty()) {
				const auto target = players.back();
				const auto& targetPhysics = entities()->get<component::Physics>(target);
				if (glm::distance(targetPhysics.position, physics.position) < 2.0f) {
					this->events()->enqueue<events::Attack>(entity, target);
				}
			}
		}
};

} // namespace shamblr
