#pragma once

#include "System.hpp"
#include "../components.hpp"
#include "../events.hpp"

namespace shamblr {

class ProjectileSystem : public System {
	public:
		ProjectileSystem() {
			auto audio = locateService<AudioService>();
			audio->createSound("pistol", "res/sound/pistol.wav");
		}
#if 1
		void configure(std::shared_ptr<EventDispatcher> events) {
			System::configure(events);
			System::events()->sink<events::RayCast>().connect<ProjectileSystem, &ProjectileSystem::receiveRayCast>(this);
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
#endif
#if 0
		void process(EntityRegistry& entities, const Time& time) {
			auto events = locateService<EventDispatcher>();

			entities()->view<component::CastedRay>().each(
				[&entities, &events](const auto entity, auto& castedRay) {
					SHAMBLR_LOG("CastedRay {%s, %s, %s, %d}\n",
						glm::to_string(castedRay.origin).c_str(),
						glm::to_string(castedRay.direction).c_str(),
						glm::to_string(castedRay.point).c_str(),
						castedRay.target
					);
#if 0
					// Apply damage to health
					if (entities()->has<component::Health>(castedRay.target)) {
						events->enqueue<events::Damage>(castedRay.target, 20);
					}
#endif
					// Remove this projectile
					entities()->destroy(entity);
				}
			);
		}
#endif
};

} // namespace shamblr
