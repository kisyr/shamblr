#include "SpriteRenderSystem.hpp"
#include "../service/CameraService.hpp"
#include "../graphics.hpp"
#include "../files.hpp"
#include "../components.hpp"

using namespace shamblr;

SpriteRenderSystem::SpriteRenderSystem() {
	const auto vSource = loadFile("res/identity.vs.glsl");
	const auto fSource = loadFile("res/identity.fs.glsl");
	const std::vector<gls::ProgramShaderInfo> shaderInfo = {
		{ GL_VERTEX_SHADER, vSource.c_str() },
		{ GL_FRAGMENT_SHADER, fSource.c_str() },
	};
	m_program = gls::ProgramCreate(shaderInfo);
}

void SpriteRenderSystem::configure() {
	entities()->construction<component::Sprite>()
		.connect<SpriteRenderSystem, &SpriteRenderSystem::constructSprite>(this);
}

void SpriteRenderSystem::enter(const Time&) {
	for (auto& b : m_batches) {
		b.second.vertices.clear();
	}
}

void SpriteRenderSystem::process(const Time& time) {
	auto camera = locateService<CameraService>();

	// Animate
	entities()->view<component::Sprite>().each(
		[this, &time](const auto entity, auto& sprite) {
			sprite.elapsed += time.delta;
			if (sprite.elapsed > 1.0f) {
				sprite.elapsed = 0.0f;
				sprite.frame = (sprite.frame + 1) % sprite.frames.size();
			}
		}
	);

	// Draw
	entities()->view<component::Sprite, component::Physics>().each(
		[this, &camera](const auto entity, const auto& sprite, const auto& physics) {
			const glm::mat4 projection = camera->projection();
			const glm::mat4 view = camera->view();
			const glm::mat4 translation = glm::translate(glm::mat4(), physics.position);			const glm::mat4 rotation = glm::mat4_cast(physics.orientation);
			const glm::mat4 scale = glm::scale(glm::mat4(), glm::vec3(physics.size / 2.0f));
			const glm::mat4 model = translation * rotation * scale;
			const glm::mat4 mvp = projection * view * model;

			const auto uv = sprite.frames[sprite.frame];
			const auto vertices = std::vector<Vertex>{
				{ mvp * glm::vec4(-1.0f, +0.0f, -1.0f, +1.0f), glm::vec2(uv.x, uv.z) },
				{ mvp * glm::vec4(-1.0f, +0.0f, +1.0f, +1.0f), glm::vec2(uv.x, uv.w) },
				{ mvp * glm::vec4(+1.0f, +0.0f, -1.0f, +1.0f), glm::vec2(uv.y, uv.z) },
				{ mvp * glm::vec4(-1.0f, +0.0f, +1.0f, +1.0f), glm::vec2(uv.x, uv.w) },
				{ mvp * glm::vec4(+1.0f, +0.0f, +1.0f, +1.0f), glm::vec2(uv.y, uv.w) },
				{ mvp * glm::vec4(+1.0f, +0.0f, -1.0f, +1.0f), glm::vec2(uv.y, uv.z) },
			};

			auto& batch = this->m_batches[sprite.batch];
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
		const auto& array = b.second.array;
		const auto& vertices = b.second.vertices;

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		SHAMBLR_LOG("Binding buffer %d\n", buffer);
		gls::BufferBind(GL_ARRAY_BUFFER, buffer);
		gls::BufferWrite(
			GL_ARRAY_BUFFER, 
			sizeof(Vertex) * vertices.size(),
			vertices.data()
		);
		gls::BufferBind(GL_ARRAY_BUFFER, 0);

		gls::VertexArrayBind(array);
		gls::TextureBind(GL_TEXTURE_2D, texture);
		gls::ProgramBind(program);
		gls::ProgramUniform("u_Sampler", 0);
		gls::DrawArrays(GL_TRIANGLES, 0, vertices.size());
		gls::ProgramBind(0);
		gls::VertexArrayBind(0);
	}
}

void SpriteRenderSystem::constructSprite(EntityRegistry&, Entity entity) {
	auto& sprite = entities()->get<component::Sprite>(entity);
	const auto spriteInfo = loadJson(sprite.description);

	// Assign sprite batch key
	sprite.batch = spriteInfo["image"];

	// Read frame data
	for (auto& f : spriteInfo["frames"]) {
		int w = spriteInfo["size"][0];
		int h = spriteInfo["size"][1];
		int u0 = f[0];
		int u1 = f[1];
		int v0 = f[2];
		int v1 = f[3];
		sprite.frames.push_back( glm::vec4(u0, v0, u1, v1) / glm::vec4(w, h, w, h));
	}

	// Initialize timing
	sprite.elapsed = 0.0f;

	if (m_batches.find(sprite.batch) == m_batches.end()) {
		m_batches[sprite.batch] = Batch();
		auto& batch = m_batches[sprite.batch];

		// Create a buffer
		batch.buffer = gls::BufferCreate(
			GL_DYNAMIC_DRAW,
			sizeof(Vertex) * 3 * 10000,
			NULL
		);

		// Create an array
		batch.array = gls::VertexArrayCreate();
		gls::BufferBind(GL_ARRAY_BUFFER, batch.buffer);
		gls::VertexArrayBind(batch.array);
		gls::VertexArrayAttributeEnable(0);
		gls::VertexArrayAttributePointer(
			0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)0);
		gls::VertexArrayAttributeEnable(1);
		gls::VertexArrayAttributePointer(
			1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)sizeof(glm::vec4));
		gls::VertexArrayBind(0);
		gls::BufferBind(GL_ARRAY_BUFFER, 0);

		// Create a texture
		Image imageFile;
		loadImage(spriteInfo["image"], imageFile);
		auto imageInfo = gls::TextureImageInfo{
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			imageFile.width, imageFile.height, 0,
			imageFile.pixels.data()
		};
		batch.texture = gls::TextureCreate2D(GL_RGBA, imageInfo);
	}
}
