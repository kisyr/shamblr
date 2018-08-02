#pragma once 

#include "System.hpp"
#include "../components.hpp"
#include "../events.hpp"

namespace shamblr {

class HealthSystem : public System {
	public:
		void configure(std::shared_ptr<EventDispatcher> events) {
			System::configure(events);
			System::events()->sink<events::Damage>().connect<HealthSystem, &HealthSystem::receiveDamage>(this);
		}

		void receiveDamage(const events::Damage& e) {
			auto& health = entities()->get<component::Health>(e.victim);
			health.current -= e.amount;
			if (health.current <= 0) {
				entities()->destroy(e.victim);
			}
		}
};

} // namespace shamblr
