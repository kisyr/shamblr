#pragma once 

#include "System.hpp"
#include "../components.hpp"
#include "../events.hpp"

namespace shamblr {

class HealthSystem : public System {
	public:
		void configure(EntityRegistry& entities) {
			m_entities = &entities;
			auto events = locateService<EventDispatcher>();
			events->sink<events::Damage>().connect<HealthSystem, &HealthSystem::receiveDamage>(this);
		}

		void receiveDamage(const events::Damage& e) {
			auto& health = m_entities->get<component::Health>(e.victim);
			health.current -= e.amount;
			if (health.current <= 0) {
				m_entities->destroy(e.victim);
			}
		}

	private:
		EntityRegistry* m_entities;
};

} // namespace shamblr
