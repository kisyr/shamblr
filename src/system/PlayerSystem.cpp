#include "PlayerSystem.hpp"
#include "../components.hpp"
#include "../service/WindowService.hpp"
#include "../service/CameraService.hpp"

using namespace shamblr;

void PlayerSystem::process(const Time& time) {
	auto window = locateService<WindowService>();
	auto camera = locateService<CameraService>();

	entities()->view<component::Player, component::Physics>().each(
		[this, &window, &camera, &time](auto entity, auto& player, auto& physics) {
#if 1
			const float rotationFactor = 3.0f * time.delta;
			float rotation = 0.0f;
			rotation += window->getKey('Z') * -rotationFactor;
			rotation += window->getKey('X') * +rotationFactor;
			physics.orientation = glm::rotate(physics.orientation, -rotation, glm::vec3(0.0f, 1.0f, 0.0f));
#endif
#if 0
			// 5 m/s running speed
			const float movementFactor = 2.5f;
			float movement = 0.0f;
			movement += window->getKey('W') * +movementFactor;
			movement += window->getKey('S') *-movementFactor;
			const glm::vec3 direction = physics.orientation * glm::vec3(0.0f, 0.0f, 1.0f);
			physics.velocity += direction * movement;
#endif
			// 5 m/s running speed
			const auto movementFactor = 2.5f;
			auto movement = glm::vec3(0.0f);
			movement.z += window->getKey('W') * +movementFactor;
			movement.z += window->getKey('S') * -movementFactor;
			movement.x += window->getKey('A') * +movementFactor;
			movement.x += window->getKey('D') * -movementFactor;
			physics.velocity += movement;
			camera->centerOn(physics.position);

			// Update aim
			const auto screenCursor = window->getMouseCursor();
			auto c0 = physics.position;
			auto c1 = camera->unProject(glm::vec3(screenCursor.x, screenCursor.y, 1.0f));
			c0.y = c1.y = 0.0f;
			player.aim = std::make_pair(c0, glm::normalize(c1 - c0));

			// Handle inventory interaction
			if (this->entities()->has<component::Inventory>(entity)) {
				auto& inventory = this->entities()->get<component::Inventory>(entity);
				// Weapon triggering
				if (!inventory.weapons.empty()) {
					inventory.weapons[0].triggered = !!window->getMouseButton(0);
				}
			}
		}
	);
}
