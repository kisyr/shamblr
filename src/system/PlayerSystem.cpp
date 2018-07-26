#include "PlayerSystem.hpp"
#include "../components.hpp"
#include "../service/InputService.hpp"
#include "../service/CameraService.hpp"

using namespace shamblr;

void PlayerSystem::process(EntityRegistry& entities, const Time& time) {
	auto input = locateService<InputService>();
	auto camera = locateService<CameraService>();

	entities.view<component::Player, component::Physics>().each(
		[&input, &camera, &time](auto entity, auto& player, auto& physics) {
			const float rotationFactor = 3.0f * time.delta;
			float rotation = 0.0f;
			rotation += input->keyState('A') * -rotationFactor;
			rotation += input->keyState('D') * +rotationFactor;
			physics.orientation = glm::rotate(physics.orientation, -rotation, glm::vec3(0.0f, 1.0f, 0.0f));

			// 5 m/s running speed
			const float movementFactor = 2.5f;
			float movement = 0.0f;
			movement += input->keyState('W') * +movementFactor;
			movement += input->keyState('S') *-movementFactor;
			const glm::vec3 direction = physics.orientation * glm::vec3(0.0f, 0.0f, 1.0f);
			physics.velocity += direction * movement;

			camera->centerOn(physics.position);
		}
	);
}
