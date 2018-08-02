#include "RenderSystem.hpp"
#include "../components.hpp"
#include "../util.hpp"
#include "../service/InputService.hpp"
#include "../service/CameraService.hpp"
#include <GLFW/glfw3.h>

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
		m_levelVertices.push_back(Vertex{v, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)});
	}
}

void RenderSystem::enter(const Time& time) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void RenderSystem::process(const Time& time) {
	auto input = locateService<InputService>();
	auto camera = locateService<CameraService>();
	auto& vertices = m_vertices;

	// Draw sprites
	entities()->view<component::Physics, component::Sprite>().each(
		[&camera, &vertices, &time](auto entity, auto& physics, auto& sprite) {
			vertices.clear();

			const float sh = 0.25f;
			const glm::vec4 color = glm::vec4(1,0,0,1);
			vertices.push_back(Vertex{glm::vec3(-sh, 0.0f, -sh), color});
			vertices.push_back(Vertex{glm::vec3(0.0f, 0.0f, +sh), color});
			vertices.push_back(Vertex{glm::vec3(+sh, 0.0f, -sh), color});

			const glm::mat4 projection = camera->projection();
			const glm::mat4 view = camera->view();
			const glm::vec3 euler = glm::eulerAngles(physics.orientation);
			const glm::mat4 translation = glm::translate(glm::mat4(), physics.position) * glm::mat4_cast(physics.orientation);
			const glm::mat4 scale = glm::scale(glm::mat4(), glm::vec3(physics.size));
			const glm::mat4 model = translation * scale;
			const glm::mat4 mvp = projection * view * model;

			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixf(&mvp[0][0]);

			glBegin(GL_TRIANGLES);
			glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
			for (auto& v : vertices) {
				glVertex3f(v.position.x, v.position.y, v.position.z);
			}
			glEnd();
		}
	);

	// Draw tracers
	entities()->view<component::Tracer>().each(
		[&camera](const auto entity, auto& tracer) {
			const auto p0 = tracer.begin;
			const auto p1 = tracer.end;

			const glm::mat4 projection = camera->projection();
			const glm::mat4 view = camera->view();
			const glm::mat4 model = glm::translate(glm::mat4(), glm::vec3(0.0f));
			const glm::mat4 mvp = projection * view * model;

			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixf(&mvp[0][0]);

			glBegin(GL_LINES);
			glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
			glVertex3f(p0.x, 0.0f, p0.z);
			glVertex3f(p1.x, 0.0f, p1.z);
			glEnd();
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

		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(&mvp[0][0]);

		glBegin(GL_TRIANGLES);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		for (auto& v : m_levelVertices) {
			glVertex3f(v.position.x, v.position.y, v.position.z);
		}
		glEnd();
	}

	// Draw player aim
	entities()->view<component::Player>().each(
		[&camera](const auto entity, auto& player) {
			const auto& aim = player.aim;
			const auto origin = aim.first;
			const auto target = aim.first + aim.second * 2.0f;

			const glm::mat4 projection = camera->projection();
			const glm::mat4 view = camera->view();
			const glm::mat4 model = glm::translate(glm::mat4(), glm::vec3(0.0f));
			const glm::mat4 mvp = projection * view * model;

			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixf(&mvp[0][0]);

			glBegin(GL_LINES);
			glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
			glVertex3f(origin.x, 0.0f, origin.z);
			glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
			glVertex3f(target.x, 0.0f, target.z);
			glEnd();
		}
	);
}

void RenderSystem::leave(const Time& time) {
}
