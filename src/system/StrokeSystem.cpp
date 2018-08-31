#include "StrokeSystem.hpp"
#include "../components.hpp"
#include "../graphics.hpp"
#include "../files.hpp"
#include "../service/CameraService.hpp"

using namespace shamblr;

void StrokeSystem::configure() {
	// Create buffer
	m_buffer = gls::BufferCreate(
		GL_STATIC_DRAW,
		sizeof(Vertex) * MaxStrokes * 2,
		NULL
	);

	// Create array
	m_array = gls::VertexArrayCreate();
	gls::BufferBind(GL_ARRAY_BUFFER, m_buffer);
	gls::VertexArrayBind(m_array);
	gls::VertexArrayAttributeEnable(0);
	gls::VertexArrayAttributePointer(
		0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)0);
	gls::VertexArrayBind(0);
	gls::BufferBind(GL_ARRAY_BUFFER, 0);

	// Create program
	const auto vSource = loadFile("res/shader/identity.vs.glsl");
	const auto fSource = loadFile("res/shader/identity.fs.glsl");
	const std::vector<gls::ProgramShaderInfo> shaderInfo = {
		{ GL_VERTEX_SHADER, vSource.c_str() },
		{ GL_FRAGMENT_SHADER, fSource.c_str() },
	};
	m_program = gls::ProgramCreate(shaderInfo);
}

void StrokeSystem::enter(const Time& time) {
	m_vertices.clear();
}

void StrokeSystem::process(const Time& time) {
	auto camera = locateService<CameraService>();

	// Draw
	entities()->view<component::Stroke>().each(
		[this, &camera](const auto entity, const auto& stroke) {
			const glm::mat4 projection = camera->projection();
			const glm::mat4 view = camera->view();
			const glm::mat4 model = glm::mat4();
			const glm::mat4 mvp = projection * view * model;

			const auto vertices = std::vector<Vertex>{
				{ mvp * glm::vec4(stroke.begin, 1.0f) },
				{ mvp * glm::vec4(stroke.end, 1.0f) },
			};

			for (auto& v : vertices) {
				m_vertices.push_back(v);
			}
		}
	);
}

void StrokeSystem::leave(const Time& time) {
	const auto& vertices = m_vertices;
	const auto& buffer = m_buffer;
	const auto& array = m_array;
	const auto& program = m_program;

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	gls::BufferBind(GL_ARRAY_BUFFER, buffer);
	gls::BufferWrite(
		GL_ARRAY_BUFFER, 
		sizeof(Vertex) * vertices.size(),
		vertices.data()
	);
	gls::BufferBind(GL_ARRAY_BUFFER, 0);

	gls::VertexArrayBind(array);
	gls::TextureBind(GL_TEXTURE_2D, 0);
	gls::ProgramBind(program);
	gls::ProgramUniform("u_Sampler", 0);
	gls::DrawArrays(GL_LINES, 0, vertices.size());
	gls::ProgramBind(0);
	gls::VertexArrayBind(0);
}
