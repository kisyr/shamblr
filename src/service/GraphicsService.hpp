#pragma once

#include "../common.hpp"
#include <string>
#include <unordered_map>

struct NVGcontext;

namespace shamblr {

class GraphicsService {
	public:
		typedef unsigned int Handle;

		GraphicsService();
		void createFont(const std::string& name, const std::string& path);
		void drawText(const std::string& font, const std::string& text, const glm::vec2& position, const float size, const glm::vec4& color);
		void drawLines(const std::vector<glm::vec3>& vertices, const glm::mat4& transform);
		void drawTriangles(const std::vector<glm::vec3>& vertices, const glm::mat4& transform);
		void flush(const glm::ivec2& screen);

	private:
		static const size_t NumLines = 100000;
		static const size_t NumTriangles = 100000;

		struct Vertex {
			glm::vec4 position;
		};

		struct DrawVerticesCommand {
			unsigned int topology;
			std::vector<glm::vec3> vertices;
			glm::mat4 transform;
		};

		struct DrawTextCommand {
			std::string font;
			std::string string;
			glm::vec2 position;
			float size;
			glm::vec4 color;
		};

		NVGcontext* m_nanovg;

		std::unordered_map<std::string, Handle> m_buffers;
		std::unordered_map<std::string, Handle> m_arrays;
		std::unordered_map<std::string, Handle> m_programs;

		std::vector<DrawVerticesCommand> m_drawVerticesCommands;
		std::vector<DrawTextCommand> m_drawTextCommands;

		std::vector<Vertex> m_lineVertices;
		std::unordered_map<std::string, std::vector<Vertex>> m_triangleVertices;
};

} // namespace shamblr
