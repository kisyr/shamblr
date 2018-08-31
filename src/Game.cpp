#include "Game.hpp"
#include "service/WindowService.hpp"
#include "service/GraphicsService.hpp"
#include "service/AudioService.hpp"
#include "service/CameraService.hpp"
#include "components.hpp"
#include "system/WorldSystem.hpp"
#include "system/SpriteSystem.hpp"
#include "system/StrokeSystem.hpp"
#include "system/LifetimeSystem.hpp"
#include "system/PhysicsSystem.hpp"
#include "system/PlayerSystem.hpp"
#include "system/BehaviourSystem.hpp"
#include "system/MovementSystem.hpp"
#include "system/HealthSystem.hpp"
//#include "system/InventorySystem.hpp"
#include "system/WeaponSystem.hpp"
#include "system/ProjectileSystem.hpp"
#include "system/HudSystem.hpp"
#include <algorithm>
#include <glm/gtc/random.hpp>

using namespace shamblr;

Game::Game(const glm::ivec2& windowSize) {
	auto window = registerService<WindowService>(windowSize);
	auto graphics = registerService<GraphicsService>();
	auto audio = registerService<AudioService>();
	auto camera = registerService<CameraService>();

	camera->viewport(glm::ivec4(0, windowSize.y, windowSize.x, -windowSize.y));

	m_entities = std::make_shared<EntityRegistry>();
	m_events = std::make_shared<EventDispatcher>();
	m_systems = std::make_shared<SystemManager>();

	configure();
}

void Game::configure() {
	m_city.generate(glm::vec2(200.0f));

	m_systems->add<WorldSystem>(m_entities, m_events, m_city);
	m_systems->add<SpriteSystem>(m_entities, m_events);
	m_systems->add<StrokeSystem>(m_entities, m_events);
	m_systems->add<LifetimeSystem>(m_entities, m_events);
	m_systems->add<PhysicsSystem>(m_entities, m_events, m_city);
	m_systems->add<PlayerSystem>(m_entities, m_events);
	m_systems->add<BehaviourSystem>(m_entities, m_events);
	m_systems->add<MovementSystem>(m_entities, m_events);
	m_systems->add<HealthSystem>(m_entities, m_events);
	//m_systems->add<InventorySystem>(m_entities, m_events);
	m_systems->add<WeaponSystem>(m_entities, m_events);
	m_systems->add<ProjectileSystem>(m_entities, m_events);
	m_systems->add<HudSystem>(m_entities, m_events);

	const auto playerRoad = glm::linearRand(0, (int)m_city.roads().size());
#if 1
	{
		const auto road = m_city.roads()[playerRoad];
		const auto position = glm::linearRand(road.min(), road.max());
		auto entity = m_entities->create();
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
		m_entities->assign<component::Sprite>(entity, "res/test.json");
		m_entities->assign<component::Player>(entity);
		m_entities->assign<component::Behaviour>(entity, component::Behaviour::Type::PLAYER);
		auto& inventory = m_entities->assign<component::Inventory>(entity);

		auto weapon = m_entities->create();
		m_entities->assign<component::Item>(weapon, entity);
		m_entities->assign<component::Weapon>(weapon, 400.0f, 800);
		inventory.items.push_back(weapon);
	}
#endif
#if 0
	auto numZombies = 0;
	for (auto& r : m_city.roads()) {
		// Down spawn zombies on player road
		if (&m_city.roads()[playerRoad] == &r) {
			continue;
		}
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
			m_entities->assign<component::Sprite>(entity, "res/test.json");
			m_entities->assign<component::Sight>(entity, 10.0f);
			m_entities->assign<component::Behaviour>(entity, component::Behaviour::Type::ZOMBIE);

			++numZombies;
		}
	}
	SHAMBLR_LOG("Spawned %d zombies\n", numZombies);
#endif
}

void Game::run() {
	auto window = locateService<WindowService>();
	auto graphics = locateService<GraphicsService>();

	float seconds = 0.0f;
	int frames = 0;
	int fps = 0.0f;

	Time time = {0};
	while (window->isOpen()) {
		const float delta = glfwGetTime();
		glfwSetTime(0);
		time.elapsed += delta;
		time.delta = delta;

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		m_systems->update(time);
		m_events->update();

		//graphics->drawText("default", tfm::format("FPS: %g", fps), glm::vec2(20.0f), 10.0f, glm::vec4(1.0f));

		//graphics->flush(window->getSize());
		window->refresh();

		seconds += time.delta;
		++frames;
		if (seconds >= 1.0f) {
			fps = (float)frames / seconds;
			seconds = 0.0f;
			frames = 0;
		}
	}
}
