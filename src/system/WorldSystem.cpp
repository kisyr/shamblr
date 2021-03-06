#include "WorldSystem.hpp"
#include "../components.hpp"
#include "../graphics.hpp"
#include "../files.hpp"
#include "../util.hpp"
#include "../service/CameraService.hpp"

using namespace shamblr;

WorldSystem::WorldSystem(
	std::shared_ptr<EntityRegistry> entities,
	std::shared_ptr<EventDispatcher> events,
	const City& city
) : System(entities, events) {
	// Create city geometry
	for (auto& r : city.lots()) {
		util::generateBoxVertices(
			glm::vec3(r.min().x, 0.0f, r.min().y),
			glm::vec3(r.max().x, 3.0f * 4, r.max().y),
			m_levelVertices
		);
	}

	// Create city buffer
	m_levelBuffer = gls::BufferCreate(
		GL_STATIC_DRAW,
		sizeof(glm::vec3) * m_levelVertices.size(),
		m_levelVertices.data()
	);

	// Create city array
	m_levelArray = gls::VertexArrayCreate();
	gls::BufferBind(GL_ARRAY_BUFFER, m_levelBuffer);
	gls::VertexArrayBind(m_levelArray);
	gls::VertexArrayAttributeEnable(0);
	gls::VertexArrayAttributePointer(
		0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (const GLvoid*)0);
	gls::VertexArrayBind(0);
	gls::BufferBind(GL_ARRAY_BUFFER, 0);

	// Create city program
	const auto vSource = loadFile("res/shader/default.vs.glsl");
	const auto fSource = loadFile("res/shader/default.fs.glsl");
	const std::vector<gls::ProgramShaderInfo> shaderInfo = {
		{ GL_VERTEX_SHADER, vSource.c_str() },
		{ GL_FRAGMENT_SHADER, fSource.c_str() },
	};
	m_levelProgram = gls::ProgramCreate(shaderInfo);
}

void WorldSystem::enter(const Time& time) {
}

void WorldSystem::process(const Time& time) {
	auto camera = locateService<CameraService>();

	const glm::mat4 projection = camera->projection();
	const glm::mat4 view = camera->view();
	const glm::mat4 translation = glm::translate(glm::mat4(), glm::vec3(0.0f));
	const glm::mat4 scale = glm::scale(glm::mat4(), glm::vec3(1.0f));
	const glm::mat4 model = translation * scale;
	const glm::mat4 mvp = projection * view * model;

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	gls::VertexArrayBind(m_levelArray);
	gls::ProgramBind(m_levelProgram);
	gls::ProgramUniform("u_Mvp", mvp);
	gls::DrawArrays(GL_TRIANGLES, 0, m_levelVertices.size());
	gls::ProgramBind(0);
	gls::VertexArrayBind(0);

	// Draw player aim
#if 0
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
#endif
}

void WorldSystem::leave(const Time& time) {
}
