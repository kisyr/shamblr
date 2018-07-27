#include "Game.hpp"
#include "service/InputService.hpp"
#include "service/CameraService.hpp"
#include "components.hpp"
#include "system/RenderSystem.hpp"
#include "system/PhysicsSystem.hpp"
#include "system/PlayerSystem.hpp"
#include "system/BehaviourSystem.hpp"
#include "system/DamageSystem.hpp"
#include "system/InventorySystem.hpp"
#include <algorithm>
#include <glm/gtc/random.hpp>

using namespace shamblr;

Game::Game() : m_window(Window(glm::ivec2(768, 768), "Shamblr")) {
	auto events = registerService<EventDispatcher>();
	auto input = registerService<InputService>(m_window.window());
	auto camera = registerService<CameraService>();

	camera->viewport(glm::ivec4(0, m_window.size().y, m_window.size().x, -m_window.size().y));

	configure();
}

void Game::run() {
	m_window.makeContextCurrent();

	Time time = {0};
	while (!m_window.shouldClose()) {
		const float delta = m_window.getTime();
		m_window.setTime(0);
		time.elapsed += delta;
		time.delta = delta;

		update(time);

		auto events = locateService<EventDispatcher>();
		events->update();

		m_window.swapBuffers();
		Window::pollEvents();
	}
}

void Game::configure() {
	m_city.generate(glm::vec2(200.0f));

	m_systems.add<RenderSystem>(m_entities, m_city);
	m_systems.add<PhysicsSystem>(m_entities, m_city);
	m_systems.add<PlayerSystem>(m_entities);
	m_systems.add<BehaviourSystem>(m_entities);
	m_systems.add<DamageSystem>(m_entities);
	m_systems.add<InventorySystem>(m_entities);

	{
		auto entity = m_entities.create();
		m_entities.assign<component::Physics>(entity, 
			glm::vec3(),
			glm::vec3(),
			glm::quat(),
			1.0f,
			0.5f,
			nullptr,
			nullptr
		);
		m_entities.assign<component::Health>(entity, 100, 100);
		m_entities.assign<component::Sprite>(entity);
		m_entities.assign<component::Player>(entity);
		m_entities.assign<component::Behaviour>(entity, component::Behaviour::Type::PLAYER);
		auto& inventory = m_entities.assign<component::Inventory>(entity);
		inventory.weapons.push_back({
			component::Inventory::Weapon::Type::HANDGUN,
			false,
			0.0f
		});
	}

#if 1
	for (auto& r : m_city.roads()) {
		if (glm::linearRand(0.0f, 1.0f) > 0.5f) {
			continue;
		}
		auto entity = m_entities.create();
		const glm::vec2 center = r.center();
		m_entities.assign<component::Physics>(entity, 
			glm::vec3(center.x, 0.0f, center.y),
			glm::vec3(),
			glm::quat(),
			1.0f,
			0.5f,
			nullptr,
			nullptr
		);
		m_entities.assign<component::Health>(entity, 100, 100);
		m_entities.assign<component::Sprite>(entity);
		m_entities.assign<component::Sight>(entity);
		m_entities.assign<component::Behaviour>(entity, component::Behaviour::Type::ZOMBIE);
	}
#endif
}

void Game::update(const Time& time) {
	m_systems.update(m_entities, time);
}
