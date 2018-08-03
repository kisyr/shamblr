#pragma once

#include "../common.hpp"

struct NVGcontext;

namespace shamblr {

class GraphicsService {
	public:
		GraphicsService();
		void createFont(const std::string& name, const std::string& path);
		void drawLines(const std::vector<glm::vec3>& vertices, const glm::mat4& transform);
		void drawTriangles(const std::vector<glm::vec3>& vertices, const glm::mat4& transform);
		void drawText(const std::string& font, const std::string& text, const glm::vec2& position, const float size, const glm::vec4& color);
		void render(const glm::ivec2& screen);

	private:
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
		std::vector<DrawVerticesCommand> m_drawVerticesCommands;
		std::vector<DrawTextCommand> m_drawTextCommands;
};

} // namespace shamblr
