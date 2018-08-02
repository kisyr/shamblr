#include "Game.hpp"
#include "service/InputService.hpp"
#include "service/CameraService.hpp"
#include "components.hpp"
#include "system/RenderSystem.hpp"
#include "system/PhysicsSystem.hpp"
#include "system/PlayerSystem.hpp"
#include "system/BehaviourSystem.hpp"
#include "system/HealthSystem.hpp"
#include "system/InventorySystem.hpp"
#include "system/ProjectileSystem.hpp"
#include <algorithm>
#include <glm/gtc/random.hpp>

using namespace shamblr;

Game::Game() : m_window(Window(glm::ivec2(1280, 1024), "Shamblr")) {
	auto input = registerService<InputService>(m_window.window());
	auto camera = registerService<CameraService>();

	camera->viewport(glm::ivec4(0, m_window.size().y, m_window.size().x, -m_window.size().y));

	m_entities = std::make_shared<EntityRegistry>();
	m_events = std::make_shared<EventDispatcher>();

	configure();
}

void Game::run() {
	m_window.makeContextCurrent();

	float seconds = 0.0f;
	int frames = 0;
	Time time = {0};
	while (!m_window.shouldClose()) {
		const float delta = m_window.getTime();
		m_window.setTime(0);
		time.elapsed += delta;
		time.delta = delta;

		m_systems.update(time);
		m_events->update();

		m_window.swapBuffers();
		Window::pollEvents();

		seconds += time.delta;
		++frames;
		if (seconds >= 1.0f) {
			SHAMBLR_LOG("FPS: %g\n", (float)frames / seconds);
			seconds = 0.0f;
			frames = 0;
		}
	}
}

void Game::configure() {
	m_city.generate(glm::vec2(200.0f));

	m_systems.add<RenderSystem>(m_entities, m_events, m_city);
	m_systems.add<PhysicsSystem>(m_entities, m_events, m_city);
	m_systems.add<PlayerSystem>(m_entities, m_events);
	m_systems.add<BehaviourSystem>(m_entities, m_events);
	m_systems.add<HealthSystem>(m_entities, m_events);
	m_systems.add<InventorySystem>(m_entities, m_events);
	m_systems.add<ProjectileSystem>(m_entities, m_events);

	{
		auto entity = m_entities->create();
		m_entities->assign<component::Physics>(entity, 
			glm::vec3(),
			glm::vec3(),
			glm::quat(),
			1.0f,
			0.5f,
			nullptr,
			nullptr
		);
		m_entities->assign<component::Health>(entity, 100, 100);
		m_entities->assign<component::Sprite>(entity);
		m_entities->assign<component::Player>(entity);
		m_entities->assign<component::Behaviour>(entity, component::Behaviour::Type::PLAYER);
		auto& inventory = m_entities->assign<component::Inventory>(entity);
		inventory.weapons.push_back({
			component::Inventory::Weapon::Type::HANDGUN,
			false,
			0.0f
		});
	}

#if 1
	auto numZombies = 0;
	for (auto& r : m_city.roads()) {
		if (glm::linearRand(0.0f, 1.0f) > 0.5f) {
			continue;
		}
		for (int n = 0; n < static_cast<int>(r.area() / 10); ++n) {
			const auto position = glm::linearRand(r.min(), r.max());
			auto entity = m_entities->create();
			const glm::vec2 center = r.center();
			m_entities->assign<component::Physics>(entity, 
				glm::vec3(position.x, 0.0f, position.y),
				glm::vec3(),
				glm::quat(),
				1.0f,
				0.5f,
				nullptr,
				nullptr
			);
			m_entities->assign<component::Health>(entity, 100, 100);
			m_entities->assign<component::Sprite>(entity);
			m_entities->assign<component::Sight>(entity);
			m_entities->assign<component::Behaviour>(entity, component::Behaviour::Type::ZOMBIE);

			++numZombies;
		}
	}
	SHAMBLR_LOG("Spawned %d zombies\n", numZombies);
#endif
}
