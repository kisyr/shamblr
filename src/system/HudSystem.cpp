#define NANOVG_GL3_IMPLEMENTATION
#include "HudSystem.hpp"
#include "../components.hpp"

using namespace shamblr;

HudSystem::HudSystem(
	std::shared_ptr<EntityRegistry> entities,
	std::shared_ptr<EventDispatcher> events
) : System(entities, events) {
	m_context = nvgCreateGL3(/*NVG_DEBUG*/ 0);
	if (!m_context) {
		throw std::runtime_error("nvgCreateGL2");
	}

	if (nvgCreateFont(m_context, "default", "res/font/TeenyTinyPixls.otf") < 0) {
		throw std::runtime_error("nvgCreateFont");
	}
}

void HudSystem::enter(const Time& time) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	const auto identity = glm::mat4();
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(&identity[0][0]);
	nvgBeginFrame(m_context, 640, 480, 1.0f);
}

void HudSystem::process(const Time& time) {
	entities()->view<component::Player, component::Health>().each(
		[this, &time](const auto entity, auto& player, auto& health) {
			const auto healthString = tfm::format("health: %d/%d",
				health.current, health.maximum);
			nvgFontFace(this->m_context, "default");
			nvgFontSize(this->m_context, 10.0f);
			nvgFillColor(this->m_context, nvgRGBAf(1.0f, 1.0f, 1.0f, 1.0f));
			nvgText(this->m_context, 20.0f, 20.0f, healthString.c_str(), NULL);
		}
	);
}

void HudSystem::leave(const Time& time) {
	nvgEndFrame(m_context);
}
