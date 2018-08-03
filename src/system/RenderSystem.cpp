#include "RenderSystem.hpp"
#include "../components.hpp"
#include "../util.hpp"
#include "../service/GraphicsService.hpp"
#include "../service/CameraService.hpp"

using namespace shamblr;

RenderSystem::RenderSystem(const City& city) {
	std::vector<glm::vec3> vertices;
	for (auto& r : city.lots()) {
		util::generateBoxVertices(
			glm::vec3(r.min().x, 0.0f, r.min().y),
			glm::vec3(r.max().x, 3.0f * 4, r.max().y),
			vertices
		);
	}
	for (auto& v : vertices) {
		m_levelVertices.push_back(v);
	}
}

void RenderSystem::enter(const Time& time) {
}

void RenderSystem::process(const Time& time) {
	auto graphics = locateService<GraphicsService>();
	auto camera = locateService<CameraService>();

	// Draw sprites
	entities()->view<component::Physics, component::Sprite>().each(
		[&graphics, &camera, &time](auto entity, auto& physics, auto& sprite) {
			const auto vertices = std::vector<glm::vec3>{
				glm::vec3(-1.0f, +0.0f, -1.0f),
				glm::vec3(+0.0f, +0.0f, +1.0f),
				glm::vec3(+1.0f, +0.0f, -1.0f),
			};

			const glm::mat4 projection = camera->projection();
			const glm::mat4 view = camera->view();
			const glm::vec3 euler = glm::eulerAngles(physics.orientation);
			const glm::mat4 translation = glm::translate(glm::mat4(), physics.position) * glm::mat4_cast(physics.orientation);
			const glm::mat4 scale = glm::scale(glm::mat4(), glm::vec3(physics.size / 2.0f));
			const glm::mat4 model = translation * scale;
			const glm::mat4 mvp = projection * view * model;

			graphics->drawTriangles(vertices, mvp);
		}
	);

	// Draw tracers
	entities()->view<component::Tracer>().each(
		[&graphics, &camera](const auto entity, auto& tracer) {
			const auto p0 = tracer.begin;
			const auto p1 = tracer.end;

			const glm::mat4 projection = camera->projection();
			const glm::mat4 view = camera->view();
			const glm::mat4 model = glm::translate(glm::mat4(), glm::vec3(0.0f));
			const glm::mat4 mvp = projection * view * model;

			graphics->drawLines(std::vector<glm::vec3>{p0, p1}, mvp);
		}
	);
	entities()->destroy<component::Tracer>();

	// Draw level
	{
		const glm::mat4 projection = camera->projection();
		const glm::mat4 view = camera->view();
		const glm::mat4 translation = glm::translate(glm::mat4(), glm::vec3(0.0f));
		const glm::mat4 scale = glm::scale(glm::mat4(), glm::vec3(1.0f));
		const glm::mat4 model = translation * scale;
		const glm::mat4 mvp = projection * view * model;

		graphics->drawTriangles(m_levelVertices, mvp);
	}

	// Draw player aim
	entities()->view<component::Player>().each(
		[&graphics, &camera](const auto entity, auto& player) {
			const auto& aim = player.aim;
			const auto origin = aim.first;
			const auto target = aim.first + aim.second * 2.0f;

			const glm::mat4 projection = camera->projection();
			const glm::mat4 view = camera->view();
			const glm::mat4 model = glm::translate(glm::mat4(), glm::vec3(0.0f));
			const glm::mat4 mvp = projection * view * model;

			graphics->drawLines(std::vector<glm::vec3>{origin, target}, mvp);
		}
	);
}

void RenderSystem::leave(const Time& time) {
}
