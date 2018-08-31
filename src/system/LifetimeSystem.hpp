#pragma once

#include "System.hpp"

namespace shamblr {

class LifetimeSystem : public System {
	public:
		void process(const Time& time) {
			entities()->view<component::Lifetime>().each(
				[this, &time](const auto entity, auto& lifetime) {
					lifetime.left -= time.delta;
					if (lifetime.left <= 0.0f) {
						this->entities()->destroy(entity);
					}
				}
			);
		}
};

} // namespace shamblr
