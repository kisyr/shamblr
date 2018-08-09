#pragma once

#include "System.hpp"
#include "../components.hpp"

namespace shamblr {

class WeaponSystem : public System {
	public:
		void process(const Time& time) {
			entities()->view<component::Item, component::Weapon>().each(
				[this, &time](const auto entity, auto& item, auto& weapon) {
					const float timeRate = 60.0f / (float)weapon.rate;
					if (item.used && weapon.fired + timeRate <= time.elapsed) {
						const auto recoil = weapon.velocity / 100.0f;
						const auto origin = weapon.origin;
						const auto cursor = weapon.origin 
							+ (weapon.direction * 100.0f) 
							+ glm::linearRand(-recoil, +recoil);
						const auto direction = glm::normalize(cursor - origin);
						const auto velocity = weapon.velocity;
						auto bullet = this->entities()->create();
						this->entities()->assign<component::Ray>(bullet, origin, direction);
						this->entities()->assign<component::Projectile>(bullet, velocity);
						weapon.fired = time.elapsed;
					}
				}
			);
		}
};

} // namespace shamblr
