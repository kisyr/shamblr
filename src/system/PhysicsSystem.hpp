#pragma once

#include "System.hpp"
#include "../City.hpp"

struct b2World;
struct b2Body;

namespace shamblr {

class PhysicsSystem : public System {
	public:
		PhysicsSystem(std::vector<Rectangle>&);
		void configure(EntityRegistry&);
		void process(EntityRegistry&, const Time&);
		void constructPhysics(EntityRegistry&, Entity);

	private:
		b2World* m_world;
		b2Body* m_ground;
		b2Body* m_level;
};

} // namespace shamblr
