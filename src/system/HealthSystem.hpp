#pragma once 

#include "System.hpp"
#include "../components.hpp"
#include "../events.hpp"

namespace shamblr {

class HealthSystem : public System {
	public:
		HealthSystem(
			std::shared_ptr<EntityRegistry> entities,
			std::shared_ptr<EventDispatcher> events
		) : System(entities, events) {
			System::events()->sink<events::Damage>()
				.connect<HealthSystem, &HealthSystem::receiveDamage>(this);
			System::events()->sink<events::WeaponFired>()
				.connect<HealthSystem, &HealthSystem::receiveWeaponFired>(this);
		}

		void process(const Time& time) {
			entities()->view<component::Health>().each(
				[&time](const auto entity, auto& health) {
					// Cap trauma to 100
					health.trauma = glm::clamp(health.trauma, 0.0f, 200.0f);
					// Remove 100 trauma in 0.5 second
					health.trauma = glm::max(0.0f, health.trauma - (200.0f * time.delta));
				}
			);
		}

		void receiveDamage(const events::Damage& e) {
			auto& health = entities()->get<component::Health>(e.victim);
			health.current -= e.amount;
			health.trauma += e.amount * 2;
			if (health.current <= 0) {
				entities()->destroy(e.victim);
			}
		}

		void receiveWeaponFired(const events::WeaponFired& e) {
			SHAMBLR_LOG("WeaponFire(owner: %d, recoil: %g)\n", e.owner, e.recoil);
			if (this->entities()->valid(e.owner)) {
				if (this->entities()->has<component::Health>(e.owner)) {
					auto& health = this->entities()->get<component::Health>(e.owner);
					health.trauma += e.recoil * 10.0f;
				}
			}
		}
};

} // namespace shamblr
