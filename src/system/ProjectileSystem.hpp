#pragma once

#include "System.hpp"
#include "../components.hpp"
#include "../events.hpp"

namespace shamblr {

class ProjectileSystem : public System {
	public:
		ProjectileSystem(
			std::shared_ptr<EntityRegistry> entities,
			std::shared_ptr<EventDispatcher> events
		) : System(entities, events) {
			auto audio = locateService<AudioService>();
			audio->createSound("pistol", "res/sound/pistol.wav");

			System::events()->sink<events::RayCast>()
				.connect<ProjectileSystem, &ProjectileSystem::receiveRayCast>(this);
		}

		void receiveRayCast(const events::RayCast& e) {
			SHAMBLR_LOG("ProjectileSystem::receiveRayCast (%s, %s, %g, %d)\n",
				glm::to_string(e.origin).c_str(),
				glm::to_string(e.direction).c_str(),
				e.length,
				e.entities.empty() ? -1 : e.entities[0]
			);

			int damage = 0;
			if (entities()->has<component::Projectile>(e.source)) {
				const auto& projectile = entities()->get<component::Projectile>(e.source);
				damage = projectile.velocity / 20;
			}

			for (auto& entity : e.entities) {
				if (entities()->valid(entity)) {
					// Apply damage to health
					if (entities()->has<component::Health>(entity)) {
						events()->enqueue<events::Damage>(entity, damage);
					}
				}
			}

			auto tracer = entities()->create();
			entities()->assign<component::Stroke>(tracer, e.origin, e.origin + e.direction * e.length);
			entities()->assign<component::Lifetime>(tracer, 2.0f);

			auto audio = locateService<AudioService>();
			audio->playSound("pistol");

			// Destroy ray
			if (entities()->valid(e.source)) {
				this->entities()->destroy(e.source);
			}
		}
};

} // namespace shamblr
