#pragma once

#include "System.hpp"
#include "../components.hpp"

namespace shamblr {

class MovementSystem : public System {
	public:
		void process(const Time& time) {
			// Process waypoints
			entities()->view<component::Physics, component::Waypoint>().each(
				[this, &time](const auto entity, auto& physics, auto& waypoint) {
					const auto direction = glm::normalize(waypoint.position - physics.position);
					const auto angle = glm::atan(direction.x, direction.z);
					const auto orientation = glm::quat(glm::vec3(0.0f, angle, 0.0f));
					const auto maxAngle = glm::pi<float>() * 0.25f;
					const auto deltaAngle = util::angleBetween(physics.orientation, orientation);
					const auto movementFactor = 1.0f - (deltaAngle / glm::half_pi<float>());
					if (glm::abs(deltaAngle) <= glm::epsilon<float>()) {
						physics.orientation = orientation;
					} else {
						const auto rotationFactor = maxAngle / deltaAngle;
						physics.orientation = glm::slerp(physics.orientation, orientation, rotationFactor * time.delta);
					}
					physics.velocity = (physics.orientation * glm::vec3(0.0f, 0.0f, 1.0f)) * movementFactor;
				}
			);
		}
};

} // namespace shamblr
