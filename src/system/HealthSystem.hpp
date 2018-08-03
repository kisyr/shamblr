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

		void process(const Time& time) {
			entities()->view<component::Health>().each(
				[&time](const auto entity, auto& health) {
					// Remove 100 trauma in 1 second
					health.trauma = glm::max(0.0f, health.trauma - ( 100.0f * time.delta));
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
};

} // namespace shamblr
