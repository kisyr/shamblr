#pragma once

#include "System.hpp"
#include "../components.hpp"

namespace shamblr {

class InventorySystem : public System {
	public:
		void process(EntityRegistry& entities, const Time& time) {
			SHAMBLR_LOG("InventorySystem::process\n");

			entities.view<component::Inventory, component::Player>().each(
				[&entities, &time](const auto entity, auto& inventory, auto& player) {
					if (!inventory.weapons.empty()) {
						auto& weapon = inventory.weapons[0];
						if (weapon.triggered) {
							if (weapon.fired + 0.3f <= time.elapsed) {
								const auto origin = player.aim.first;
								const auto direction = player.aim.second;
								const auto velocity = 1000.0f;
								const auto diameter = 9.0f;
								const auto length = 19.0f;
								auto bullet = entities.create();
								entities.assign<component::Projectile>(bullet, origin, direction, velocity);
								entities.assign<component::Caliber>(bullet, diameter, length);
								weapon.fired = time.elapsed;
							}
						}
					}
				}
			);
		}
};

} // namespace shamblr
