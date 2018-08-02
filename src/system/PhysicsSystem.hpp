#pragma once

#include "System.hpp"
#include "../City.hpp"

struct b2World;
struct b2Body;

namespace shamblr {

class PhysicsSystem : public System {
	public:
		PhysicsSystem(const City&);
		void configure(std::shared_ptr<EntityRegistry>);
		void process(const Time&);
		void constructPhysics(EntityRegistry&, Entity);
		void destructPhysics(EntityRegistry&, Entity);

	private:
		b2World* m_world;
		b2Body* m_ground;
		b2Body* m_level;
};

} // namespace shamblr
