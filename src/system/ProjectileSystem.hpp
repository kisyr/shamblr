#pragma once

#include "System.hpp"
#include "../components.hpp"
#include "../events.hpp"

namespace shamblr {

class ProjectileSystem : public System {
	public:
#if 1
		void configure(EntityRegistry& entities) {
			m_entities = &entities;
			auto events = locateService<EventDispatcher>();
			events->sink<events::RayCast>().connect<ProjectileSystem, &ProjectileSystem::receiveRayCast>(this);
		}

		void receiveRayCast(const events::RayCast& e) {
			SHAMBLR_LOG("ProjectileSystem::receiveRayCast (%s, %s, %g, %d)\n",
				glm::to_string(e.origin).c_str(),
				glm::to_string(e.direction).c_str(),
				e.length,
				e.entities.empty() ? -1 : e.entities[0]
			);

			auto events = locateService<EventDispatcher>();

			for (auto& entity : e.entities) {
				if (m_entities->valid(entity)) {
					// Apply damage to health
					if (m_entities->has<component::Health>(entity)) {
						events->enqueue<events::Damage>(entity, 20);
					}
				}
			}

			auto tracer = m_entities->create();
			m_entities->assign<component::Tracer>(tracer, e.origin, e.origin + e.direction * e.length);
		}
#endif
#if 0
		void process(EntityRegistry& entities, const Time& time) {
			auto events = locateService<EventDispatcher>();

			entities.view<component::CastedRay>().each(
				[&entities, &events](const auto entity, auto& castedRay) {
					SHAMBLR_LOG("CastedRay {%s, %s, %s, %d}\n",
						glm::to_string(castedRay.origin).c_str(),
						glm::to_string(castedRay.direction).c_str(),
						glm::to_string(castedRay.point).c_str(),
						castedRay.target
					);
#if 0
					// Apply damage to health
					if (entities.has<component::Health>(castedRay.target)) {
						events->enqueue<events::Damage>(castedRay.target, 20);
					}
#endif
					// Remove this projectile
					entities.destroy(entity);
				}
			);
		}
#endif

	private:
		EntityRegistry* m_entities;
};

} // namespace shamblr
