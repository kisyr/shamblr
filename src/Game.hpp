#pragma once

#include "common.hpp"
#include "components.hpp"
#include "RenderSystem.hpp"

namespace shamblr {

class Game {
	public:
		void configure() {
			Entity entity = m_registry.create();
			m_registry.assign<Position>(entity);
			m_registry.assign<Velocity>(entity);
			m_registry.assign<Health>(entity, 100, 100);
			m_registry.assign<Sprite>(entity);
		}

		void update(const Time& time, const Input& input) {
			m_render.update(m_registry, time);
		}

	private:
		EntityRegistry m_registry;
		RenderSystem m_render;
};

} // namespace shamblr
