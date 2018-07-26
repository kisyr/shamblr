#pragma once 

#include "System.hpp"
#include "../components.hpp"
#include "../events.hpp"

namespace shamblr {

class DamageSystem : public System {
	public:
		void configure(EntityRegistry& entities) {
			m_entities = &entities;
			auto events = locateService<EventDispatcher>();
			events->sink<events::Attack>().connect<DamageSystem, &DamageSystem::receiveAttack>(this);
		}

		void receiveAttack(const events::Attack& e) {
			SHAMBLR_LOG("events::Attack: %d, %d\n", e.origin, e.target);
		}

	private:
		EntityRegistry* m_entities;
};

} // namespace shamblr
