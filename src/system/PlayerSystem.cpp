#include "PlayerSystem.hpp"
#include "../components.hpp"
#include "../service/InputService.hpp"
#include "../service/CameraService.hpp"

using namespace shamblr;

void PlayerSystem::process(EntityRegistry& entities, const Time& time) {
	auto input = locateService<InputService>();
	auto camera = locateService<CameraService>();

	entities.view<component::Player, component::Physics>().each(
		[&input, &camera, &entities, &time](auto entity, auto& player, auto& physics) {
#if 0
			const float rotationFactor = 3.0f * time.delta;
			float rotation = 0.0f;
			rotation += input->getKey('A') * -rotationFactor;
			rotation += input->getKey('D') * +rotationFactor;
			physics.orientation = glm::rotate(physics.orientation, -rotation, glm::vec3(0.0f, 1.0f, 0.0f));

			// 5 m/s running speed
			const float movementFactor = 2.5f;
			float movement = 0.0f;
			movement += input->getKey('W') * +movementFactor;
			movement += input->getKey('S') *-movementFactor;
			const glm::vec3 direction = physics.orientation * glm::vec3(0.0f, 0.0f, 1.0f);
			physics.velocity += direction * movement;
#endif
			// 5 m/s running speed
			const auto movementFactor = 2.5f;
			auto movement = glm::vec3(0.0f);
			movement.z += input->getKey('W') * +movementFactor;
			movement.z += input->getKey('S') * -movementFactor;
			movement.x += input->getKey('A') * +movementFactor;
			movement.x += input->getKey('D') * -movementFactor;
			physics.velocity += movement;
			camera->centerOn(physics.position);

			// Update aim
			const auto screenCursor = input->getMouseCursor();
			auto c0 = physics.position;
			auto c1 = camera->unProject(glm::vec3(screenCursor.x, screenCursor.y, 1.0f));
			c0.y = c1.y = 0.0f;
			player.aim = std::make_pair(c0, glm::normalize(c1 - c0));

			// Handle inventory interaction
			if (entities.has<component::Inventory>(entity)) {
				auto& inventory = entities.get<component::Inventory>(entity);
				// Weapon triggering
				if (!inventory.weapons.empty()) {
					inventory.weapons[0].triggered = !!input->getMouseButton(0);
				}
			}
		}
	);
}
