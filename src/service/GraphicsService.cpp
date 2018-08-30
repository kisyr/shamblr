#include "GraphicsService.hpp"
#define NANOVG_GL2_IMPLEMENTATION
#include <fstream>
#include "../graphics.hpp"
#include "../files.hpp"

using namespace shamblr;

GraphicsService::GraphicsService() : m_nanovg(NULL) {
	// Initialize nanovg
	m_nanovg = nvgCreateGL2(NVG_STENCIL_STROKES | NVG_DEBUG);
	if (!m_nanovg) {
		throw std::runtime_error("nvgCreateGL2");
	}

	// Create default font
	createFont("default", "res/TeenyTinyPixls.otf");
#if 0
	// Create line batch
	{
		// Buffer
		m_buffers["lines"] = gls::BufferCreate(
			GL_DYNAMIC_DRAW,
			sizeof(Vertex) * 2 * NumLines,
			NULL
		);
		// Program
		const auto vSource = loadFile("res/identity.vs.glsl");
		const auto fSource = loadFile("res/identity.fs.glsl");
		const std::vector<gls::ProgramShaderInfo> shaders = {
			{ GL_VERTEX_SHADER, vSource.c_str() },
			{ GL_FRAGMENT_SHADER, fSource.c_str() },
		};
		m_programs["lines"] = gls::ProgramCreate(shaders);
	}

	// Create triangle batch
	{
		// Buffer
		m_buffers["triangles"] = gls::BufferCreate(
			GL_DYNAMIC_DRAW,
			sizeof(Vertex) * 3 * NumTriangles,
			NULL
		);
		// Program
		const auto vSource = loadFile("res/identity.vs.glsl");
		const auto fSource = loadFile("res/identity.fs.glsl");
		const std::vector<gls::ProgramShaderInfo> shaders = {
			{ GL_VERTEX_SHADER, vSource.c_str() },
			{ GL_FRAGMENT_SHADER, fSource.c_str() },
		};
		m_programs["triangles"] = gls::ProgramCreate(shaders);
	}
#endif
}

void GraphicsService::createFont(const std::string& name, const std::string& path) {
	nvgCreateFont(m_nanovg, name.c_str(), path.c_str());
}

void GraphicsService::drawLines(const std::vector<glm::vec3>& vertices, const glm::mat4& transform) {
	for (auto& v : vertices) {
		m_lineVertices.push_back(Vertex{ transform * glm::vec4(v, 1.0f) });
	}
}

void GraphicsService::drawTriangles(const std::vector<glm::vec3>& vertices, const glm::mat4& transform) {
	for (auto& v : vertices) {
		m_triangleVertices["default"].push_back(Vertex{ transform * glm::vec4(v, 1.0f) });
	}
}

void GraphicsService::drawText(const std::string& font, const std::string& text, const glm::vec2& position, const float size, const glm::vec4& color) {
	m_drawTextCommands.push_back(DrawTextCommand{font, text, position, size, color});
}

void GraphicsService::flush(const glm::ivec2& screen) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
#if 0
	// Draw lines
	{
		const auto& buffer = m_buffers["lines"];
		const auto& program = m_programs["lines"];
		auto& vertices = m_lineVertices;

		gls::BufferBind(GL_ARRAY_BUFFER, buffer);
		gls::BufferWrite(
			GL_ARRAY_BUFFER, 
			sizeof(Vertex) * vertices.size(),
			vertices.data()
		);
		gls::VertexArrayAttributeEnable(0);
		gls::VertexArrayAttributePointer(
			0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		gls::ProgramBind(program);
		gls::DrawArrays(GL_LINES, 0, vertices.size());
		gls::ProgramBind(0);
		gls::BufferBind(GL_ARRAY_BUFFER, 0);
		vertices.clear();
	}

	// Draw triangles
	for (auto& p : m_triangleVertices) {
		if (p.second.empty()) {
			continue;
		}

		const auto& buffer = m_buffers["triangles"];
		const auto& program = m_programs["identity"];
		auto& vertices = p.second;

		gls::BufferBind(GL_ARRAY_BUFFER, buffer);
		gls::BufferWrite(
			GL_ARRAY_BUFFER, 
			sizeof(Vertex) * vertices.size(),
			vertices.data()
		);
		gls::VertexArrayAttributeEnable(0);
		gls::VertexArrayAttributePointer(
			0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		gls::ProgramBind(program);
		gls::DrawArrays(GL_TRIANGLES, 0, vertices.size());
		gls::ProgramBind(0);
		gls::BufferBind(GL_ARRAY_BUFFER, 0);
		vertices.clear();
	}
#endif
	// Draw fonts
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	const auto identity = glm::mat4();
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(&identity[0][0]);
	nvgBeginFrame(m_nanovg, screen.x, screen.y, 1.0f);
	for (auto& dtc : m_drawTextCommands) {
		nvgFontFace(m_nanovg, dtc.font.c_str());
		nvgFontSize(m_nanovg, dtc.size);
		nvgFillColor(m_nanovg, nvgRGBAf(dtc.color.x, dtc.color.y, dtc.color.z, dtc.color.w));
		nvgText(m_nanovg, dtc.position.x, dtc.position.y, dtc.string.c_str(), NULL);
	}
	nvgEndFrame(m_nanovg);
}
