#pragma once

#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include "../common.hpp"

namespace shamblr {

class System {
	public:
		virtual ~System() {}
		virtual void enter(const Time&) {}
		virtual void process(const Time&) {}
		virtual void leave(const Time&) {}

		virtual void configure(std::shared_ptr<EntityRegistry> entities) {
			m_entities = entities;
		}

		virtual void configure(std::shared_ptr<EventDispatcher> events) {
			m_events = events;
		}

		virtual void update(const Time& time) {
			enter(time);
			process(time);
			leave(time);
		}

		std::shared_ptr<EntityRegistry> entities() {
			return m_entities;
		}

		std::shared_ptr<EventDispatcher> events() {
			return m_events;
		}

	private:
		std::shared_ptr<EntityRegistry> m_entities;
		std::shared_ptr<EventDispatcher> m_events;
};

class SystemManager {
	public:
		template <class T, class... Args>
		void add(std::shared_ptr<EntityRegistry> entities, std::shared_ptr<EventDispatcher> events, Args... args) {
			m_systems[typeid(T)] = std::make_shared<T>(args...);
			m_systems[typeid(T)]->configure(entities);
			m_systems[typeid(T)]->configure(events);
		}

		template <class T>
		void update(const Time& time) {
			m_systems[typeid(T)]->update(time);
		}

		void update(const Time& time) {
			for (auto& s : m_systems) {
				s.second->update(time);
			}
		}

	private:
		std::unordered_map<std::type_index, std::shared_ptr<System>> m_systems;
};

} // namespace shamblr
