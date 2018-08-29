#include "SpriteRenderSystem.hpp"
#include "../service/CameraService.hpp"
#include "../graphics.hpp"
#include "../files.hpp"
#include "../components.hpp"

using namespace shamblr;

SpriteRenderSystem::SpriteRenderSystem() {
	const auto spriteInfo = loadJson("res/test.sprite.json");
	const std::string spriteInfoFile = spriteInfo["file"];
	SHAMBLR_LOG("Sprite (%s)\n", spriteInfoFile);

	Batch batch;

	batch.buffer = gls::BufferCreate(
		GL_DYNAMIC_DRAW,
		sizeof(Vertex) * 3 * 10000,
		NULL
	);

	const auto imageName = "res/Untitled-10.png";
	Image imageFile;
	loadImage(imageName, imageFile);
	auto imageInfo = gls::TextureImageInfo{
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		imageFile.width, imageFile.height, 0,
		imageFile.pixels.data()
	};
	batch.texture = gls::TextureCreate2D(GL_RGBA, imageInfo);

	m_batches["default"] = batch;

	const auto vSource = loadFile("res/identity.vs.glsl");
	const auto fSource = loadFile("res/identity.fs.glsl");
	const std::vector<gls::ProgramShaderInfo> shaderInfo = {
		{ GL_VERTEX_SHADER, vSource.c_str() },
		{ GL_FRAGMENT_SHADER, fSource.c_str() },
	};
	m_program = gls::ProgramCreate(shaderInfo);
}

void SpriteRenderSystem::enter(const Time&) {
	for (auto& b : m_batches) {
		b.second.vertices.clear();
	}
}

void SpriteRenderSystem::process(const Time& time) {
	auto camera = locateService<CameraService>();

	entities()->view<component::Sprite, component::Physics>().each(
		[this, &camera](const auto entity, const auto& sprite, const auto& physics) {
			const glm::mat4 projection = camera->projection();
			const glm::mat4 view = camera->view();
			const glm::mat4 translation = glm::translate(glm::mat4(), physics.position);			const glm::mat4 rotation = glm::mat4_cast(physics.orientation);
			const glm::mat4 scale = glm::scale(glm::mat4(), glm::vec3(physics.size / 2.0f));
			const glm::mat4 model = translation * rotation * scale;
			const glm::mat4 mvp = projection * view * model;

			const auto vertices = std::vector<Vertex>{
				{ mvp * glm::vec4(-1.0f, +0.0f, -1.0f, +1.0f), glm::vec2(0.0f, 1.0f) },
				{ mvp * glm::vec4(-1.0f, +0.0f, +1.0f, +1.0f), glm::vec2(0.0f, 0.0f) },
				{ mvp * glm::vec4(+1.0f, +0.0f, -1.0f, +1.0f), glm::vec2(1.0f, 1.0f) },
				{ mvp * glm::vec4(-1.0f, +0.0f, +1.0f, +1.0f), glm::vec2(0.0f, 0.0f) },
				{ mvp * glm::vec4(+1.0f, +0.0f, +1.0f, +1.0f), glm::vec2(1.0f, 0.0f) },
				{ mvp * glm::vec4(+1.0f, +0.0f, -1.0f, +1.0f), glm::vec2(1.0f, 1.0f) },
			};

			auto& batch = this->m_batches["default"];
			for (auto& v : vertices) {
				batch.vertices.push_back(v);
			}
		}
	);
}

void SpriteRenderSystem::leave(const Time&) {
	const auto& program = m_program;

	for (auto& b : m_batches) {
		const auto& texture = b.second.texture;
		const auto& buffer = b.second.buffer;
		const auto& vertices = b.second.vertices;

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		gls::BufferBind(GL_ARRAY_BUFFER, buffer);
		gls::BufferWrite(
			GL_ARRAY_BUFFER, 
			sizeof(Vertex) * vertices.size(),
			vertices.data()
		);
		gls::VertexArrayAttributeEnable(0);
		gls::VertexArrayAttributePointer(
			0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		gls::VertexArrayAttributeEnable(1);
		gls::VertexArrayAttributePointer(
			1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)sizeof(glm::vec4));
		gls::TextureBind(GL_TEXTURE_2D, texture);
		gls::ProgramBind(program);
		gls::ProgramUniform("u_Sampler", 0);
		gls::DrawArrays(GL_TRIANGLES, 0, vertices.size());
		gls::ProgramBind(0);
		gls::BufferBind(GL_ARRAY_BUFFER, 0);
	}
}
