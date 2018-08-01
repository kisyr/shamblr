#pragma once

#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include "../common.hpp"

namespace shamblr {

class System {
	public:
		virtual void configure(EntityRegistry&) {}
		virtual void enter(const Time&) {}
		virtual void process(EntityRegistry&, const Time&) {}
		virtual void leave(const Time&) {}

		virtual void update(EntityRegistry& entities, const Time& time) {
			enter(time);
			process(entities, time);
			leave(time);
		}
};

class SystemManager {
	public:
		template <class T, class... Args>
		void add(EntityRegistry& entities, Args... args) {
			m_systems[typeid(T)] = std::make_shared<T>(args...);
			m_systems[typeid(T)]->configure(entities);
		}

		template <class T>
		void update(EntityRegistry& entities, const Time& time) {
			m_systems[typeid(T)]->update(entities, time);
		}

		void update(EntityRegistry& entities, const Time& time) {
			for (auto& s : m_systems) {
				s.second->update(entities, time);
			}
		}

	private:
		std::unordered_map<std::type_index, std::shared_ptr<System>> m_systems;
};

} // namespace shamblr
