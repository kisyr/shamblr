#pragma once 

#include "System.hpp"
#include "../components.hpp"
#include "../events.hpp"

namespace shamblr {

class DamageSystem : public System {
	public:
		void configure(std::shared_ptr<EventDispatcher> events) {
			System::configure(events);
			events()->sink<events::Attack>().connect<DamageSystem, &DamageSystem::receiveAttack>(this);
			events()->sink<events::ProjectileHit>().connect<DamageSystem, &DamageSystem::receiveProjectileHit>(this);
		}

		void receiveAttack(const events::Attack& e) {
			SHAMBLR_LOG("events::Attack: %d, %d\n", e.origin, e.target);
		}

		void receiveProjectileHit(const events::ProjectileHit& e) {
			SHAMBLR_LOG("Projectile hit %d\n", e.victim);
			if (entities()->has<component::Health>(e.victim)) {
				auto& health = entities()->get<component::Health>(e.victim);
				health.current -= 20;
				if (health.current <= 0) {
					SHAMBLR_LOG("Projectile killed %d\n", e.victim);
					entities()->destroy(e.victim);
				}
			}
		}
};

} // namespace shamblr
