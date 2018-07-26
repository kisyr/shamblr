#include "RenderSystem.hpp"
#include "../components.hpp"
#include "../service/InputService.hpp"
#include "../service/CameraService.hpp"
#include <GLFW/glfw3.h>

using namespace shamblr;

void RenderSystem::enter(const Time& time) {
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void RenderSystem::process(EntityRegistry& entities, const Time& time) {
	auto input = locateService<InputService>();
	auto camera = locateService<CameraService>();
	auto& vertices = m_vertices;

	entities.view<component::Physics, component::Sprite>().each(
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
			for (auto& v : vertices) {
				glVertex3f(v.position.x, v.position.y, v.position.z);
			}
			glEnd();
		}
	);
}

void RenderSystem::leave(const Time& time) {
	auto camera = locateService<CameraService>();

	const glm::mat4 projection = camera->projection();
	const glm::mat4 view = camera->view();
	const glm::mat4 model = glm::mat4();
	const glm::mat4 mvp = projection * view * model;

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(&mvp[0][0]);
}
