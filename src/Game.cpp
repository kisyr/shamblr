#include "Game.hpp"
#include "service/InputService.hpp"
#include "service/CameraService.hpp"
#include "components.hpp"
#include "system/RenderSystem.hpp"
#include "system/PhysicsSystem.hpp"
#include "system/PlayerSystem.hpp"
#include "system/BehaviourSystem.hpp"
#include "system/DamageSystem.hpp"
#include <algorithm>
#include <glm/gtc/random.hpp>

using namespace shamblr;

Game::Game() : m_window(Window(glm::ivec2(1920, 1080), "Shamblr")) {
	auto events = registerService<EventDispatcher>();
	auto input = registerService<InputService>(m_window.window());
	auto camera = registerService<CameraService>();

	camera->viewport(glm::ivec4(0, 0, m_window.size().x, m_window.size().y));

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

		glBegin(GL_TRIANGLES);
		for (auto& r : m_city.lots()) {
			glVertex3f(r.min().x, 0.0f, r.min().y);
			glVertex3f(r.min().x, 0.0f, r.max().y);
			glVertex3f(r.max().x, 0.0f, r.min().y);
			glVertex3f(r.max().x, 0.0f, r.max().y);
			glVertex3f(r.min().x, 0.0f, r.max().y);
			glVertex3f(r.max().x, 0.0f, r.min().y);
		}
		glEnd();

		m_window.swapBuffers();
		Window::pollEvents();
	}
}

void Game::configure() {
	m_city.generate(glm::vec2(50.0f));

	m_systems.add<RenderSystem>(m_entities);
	m_systems.add<PhysicsSystem>(m_entities, m_city.lots());
	m_systems.add<PlayerSystem>(m_entities);
	m_systems.add<BehaviourSystem>(m_entities);
	m_systems.add<DamageSystem>(m_entities);

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
	}

	if (false) {
		auto entity = m_entities.create();
		m_entities.assign<component::Physics>(entity, 
			glm::vec3(0.0f, 0.0f, 10.0f),
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

	for (auto& r : m_city.roads()) {
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
}

void Game::update(const Time& time) {
	m_systems.update(m_entities, time);
}
