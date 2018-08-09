#pragma once

#include "System.hpp"
#include "../components.hpp"

namespace shamblr {

class InventorySystem : public System {
	public:
		void process(const Time& time) {
#if 0
			entities()->view<component::Inventory, component::Player, component::Health>().each(
				[this, &time](const auto entity, auto& inventory, auto& player, auto& health) {
					if (!inventory.weapons.empty()) {
						auto& weapon = inventory.weapons[0];
						if (weapon.triggered) {
							if (weapon.fired + 0.3f <= time.elapsed) {
								const auto origin = player.aim.first;
								const auto cursor = player.aim.first + (player.aim.second * 100.0f) + glm::linearRand(-10.0f, +10.0f);
								const auto direction = glm::normalize(cursor - origin);
								const auto velocity = 1000.0f;
								const auto diameter = 9.0f;
								const auto length = 19.0f;
								auto bullet = this->entities()->create();
								this->entities()->assign<component::Ray>(bullet, origin, direction);
								this->entities()->assign<component::Caliber>(bullet, diameter, length);
								weapon.fired = time.elapsed;
								health.trauma += 40.0f;
							}
						}
					}
				}
			);
#endif
		}
};

} // namespace shamblr
