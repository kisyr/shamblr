#pragma once

#include "System.hpp"
#include "../common.hpp"
#include "../components.hpp"

namespace shamblr {

class PlayerSystem : public System {
	public:
		struct Control {
			std::string key;
			std::string button;
			std::string action;
		};

		PlayerSystem(
			std::shared_ptr<EntityRegistry>,
			std::shared_ptr<EventDispatcher>,
			const std::vector<Control>& controls
		);
		void process(const Time&);

	private:
		std::vector<Control> m_controls;
};

} // namespace shamblr
