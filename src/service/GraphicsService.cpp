#include "GraphicsService.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define NANOVG_GL2_IMPLEMENTATION
#include <nanovg.h>
#include <nanovg_gl.h>

using namespace shamblr;

GraphicsService::GraphicsService() : m_nanovg(NULL) {
	m_nanovg = nvgCreateGL2(NVG_STENCIL_STROKES | NVG_DEBUG);
	if (!m_nanovg) {
		throw std::runtime_error("nvgCreateGL2");
	}
	createFont("default", "res/TeenyTinyPixls.otf");
}

void GraphicsService::createFont(const std::string& name, const std::string& path) {
	nvgCreateFont(m_nanovg, name.c_str(), path.c_str());
}

void GraphicsService::drawLines(const std::vector<glm::vec3>& vertices, const glm::mat4& transform) {
	m_drawVerticesCommands.push_back(DrawVerticesCommand{GL_LINES, vertices, transform});
}

void GraphicsService::drawTriangles(const std::vector<glm::vec3>& vertices, const glm::mat4& transform) {
	m_drawVerticesCommands.push_back(DrawVerticesCommand{GL_TRIANGLES, vertices, transform});
}

void GraphicsService::drawText(const std::string& font, const std::string& text, const glm::vec2& position, const float size, const glm::vec4& color) {
	m_drawTextCommands.push_back(DrawTextCommand{font, text, position, size, color});
}

void GraphicsService::render(const glm::ivec2& screen) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	for (auto& dvc : m_drawVerticesCommands) {
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(&dvc.transform[0][0]);
		glBegin(dvc.topology);
		for (auto& v : dvc.vertices) {
			glVertex3f(v.x, v.y, v.z);
		}
		glEnd();
	}
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
	m_drawVerticesCommands.clear();
	m_drawTextCommands.clear();
}
