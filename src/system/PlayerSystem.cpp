#include "PlayerSystem.hpp"
#include "../components.hpp"
#include "../service/WindowService.hpp"
#include "../service/CameraService.hpp"
#include <glm/gtc/noise.hpp>

using namespace shamblr;

PlayerSystem::PlayerSystem(
	std::shared_ptr<EntityRegistry> entities,
	std::shared_ptr<EventDispatcher> events,
	const std::vector<Control>& controls
) : System(entities, events), m_controls(controls) {}

void PlayerSystem::process(const Time& time) {
	auto window = locateService<WindowService>();
	auto camera = locateService<CameraService>();

	entities()->view<component::Player, component::Physics, component::Health>().each(
		[this, &window, &camera, &time](auto entity, auto& player, auto& physics, auto& health) {
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
			const auto movementFactor = 2.5f + (window->getKey(GLFW_KEY_LEFT_SHIFT) * 2.5f);
			auto movement = glm::vec3(0.0f);
			for (auto& c : m_controls) {
				switch (true) {
					case c.action == "moveUp":
						movement.z += window->getKey(c.key.c_str().at(0)) * +movementFactor;
						break;
					case c.action == "moveDown":
						movement.z += window->getKey(c.key.c_str().at(0)) * -movementFactor;
						break;
					case c.action == "moveLeft":
						movement.x += window->getKey(c.key.c_str().at(0)) * +movementFactor;
						break;
					case c.action == "moveRight":
						movement.x += window->getKey(c.key.c_str().at(0)) * -movementFactor;
						break;
				}
			}
			physics.velocity += movement;

			// Update aim
			const auto screenCursor = window->getMouseCursor();
			auto c0 = physics.position;
			auto c1 = camera->unProject(glm::vec3(screenCursor.x, screenCursor.y, 1.0f));
			c0.y = c1.y = 0.0f;
			player.aim = std::make_pair(c0, glm::normalize(c1 - c0));

			// Handle inventory
			if (this->entities()->has<component::Inventory>(entity)) {
				auto& inventory = this->entities()->get<component::Inventory>(entity);
				// Update weapon spatials
				for (const auto& item : inventory.items) {
					if (this->entities()->has<component::Weapon>(item)) {
						auto& weapon = this->entities()->get<component::Weapon>(item);
						weapon.origin = player.aim.first;
						weapon.direction = player.aim.second;
					}
				}
				// Item triggering
				if (!inventory.items.empty()) {
					const auto inventoryItem = inventory.items.front();
					auto& item = this->entities()->get<component::Item>(inventoryItem);
					item.used = !!window->getMouseButton(0);
				}
			}

			if (window->getKey('T')) {
				health.trauma = 100.0f;
			}

			// Camera follows player
			auto cameraCenter = physics.position;
			const auto traumaFactor = glm::pow(health.trauma, 3) * 0.000001f;
			cameraCenter.x += glm::simplex(glm::vec2(time.elapsed, 0.0f)) * traumaFactor;
			cameraCenter.z += glm::simplex(glm::vec2(0.0f, time.elapsed)) * traumaFactor;
			camera->centerOn(cameraCenter);
		}
	);
}
