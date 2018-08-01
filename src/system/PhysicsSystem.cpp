#include "PhysicsSystem.hpp"
#include "../components.hpp"
#include "../events.hpp"
#include <Box2D/Box2D.h>

using namespace shamblr;

class LineOfSight : public b2RayCastCallback {
	public:
		LineOfSight(const b2Fixture* origin, const b2Fixture* target) : 
			m_origin(origin), m_target(target), m_sight(false) {}

		float32 ReportFixture(
			b2Fixture* fixture,
			const b2Vec2& point,
			const b2Vec2& normal,
			float32 fraction
		) {
			m_closest = fixture;
			if (fixture == m_origin) {
				return -1.0f;
			}
			return fraction;
		}

		bool hasSight() const {
			return m_closest == m_target;
		}

	private:
		const b2Fixture* m_origin;
		const b2Fixture* m_target;
		b2Fixture* m_closest;
		bool m_sight;
};

class RayCastClosest : public b2RayCastCallback {
	public:
		RayCastClosest() : m_closest(NULL) {}

		float32 ReportFixture(
			b2Fixture* fixture,
			const b2Vec2& point,
			const b2Vec2& normal,
			float32 fraction
		) {
			m_closest = fixture;
			return fraction;
		}

		Entity* hitEntity() const {
			return m_closest && m_closest->GetUserData() ? static_cast<Entity*>(m_closest->GetUserData()) : NULL;
		}

	private:
		b2Fixture* m_closest;
};

PhysicsSystem::PhysicsSystem(const City& city) {
	{
		m_world = new b2World(b2Vec2(0.0f, 0.0f));
		b2BodyDef groundDef;
		m_ground = m_world->CreateBody(&groundDef);
	}

	{
		b2BodyDef bodyDef;
		bodyDef.type = b2_staticBody;
		bodyDef.position.Set(0.0f, 0.0f);
		b2Body* body = m_world->CreateBody(&bodyDef);

		for (auto& b : city.lots()) {
			b2Vec2 vertices[] = {
				b2Vec2(b.min().x, b.min().y),
				b2Vec2(b.min().x, b.max().y),
				b2Vec2(b.max().x, b.max().y),
				b2Vec2(b.max().x, b.min().y),
			};

			b2PolygonShape shape;
			shape.Set(vertices, 4);

			b2FixtureDef fixtureDef;
			fixtureDef.shape = &shape;
			fixtureDef.density = 1.0f;
			fixtureDef.friction = 1.0f;

			body->CreateFixture(&fixtureDef);
		}

		m_level = body;
	}
}

void PhysicsSystem::configure(EntityRegistry& entities) {
	entities.construction<component::Physics>().connect<PhysicsSystem, &PhysicsSystem::constructPhysics>(this);
	entities.destruction<component::Physics>().connect<PhysicsSystem, &PhysicsSystem::destructPhysics>(this);
}

void PhysicsSystem::process(EntityRegistry& entities, const Time& time) {
	auto events = locateService<EventDispatcher>();

	// Apply forces to b2d
	entities.view<component::Physics>().each(
		[](auto entity, auto& physics) {
			auto& velocity = physics.velocity;
			physics.body->SetLinearVelocity(b2Vec2(velocity.x, velocity.z));
			velocity = glm::vec3();
		}
	);

	// Simulate b2d world
	m_world->Step(1.0f / 60.0f, 6, 2);

	// Sync spatials with b2d world
	entities.view<component::Physics>().each(
		[](auto entity, auto& physics) {
			const auto b2Position = physics.body->GetPosition();
			physics.position = glm::vec3(b2Position.x, 0.0f, b2Position.y);
		}
	);

	// Process sight
	auto world = m_world;
	entities.view<component::Physics, component::Sight>().each(
		[&world, &entities](const auto entityA, auto& physicsA, auto& sightA) {
			// Reset sights
			sightA.entities.clear();
			// Check for line of sight against others
			entities.view<component::Physics>().each(
				[world, &entities, &entityA, &physicsA, &sightA](const auto entityB, auto& physicsB) {
					if (entityA != entityB) {
						auto los = LineOfSight(physicsA.fixture, physicsB.fixture);
						auto p0 = b2Vec2(physicsA.position.x, physicsA.position.z);
						auto p1 = b2Vec2(physicsB.position.x, physicsB.position.z);
						world->RayCast(&los, p0, p1);
						if (los.hasSight()) {
							sightA.entities.push_back(entityB);
						}
					}
				}
			);
		}
	);

	// Process projectiles
	entities.view<component::Projectile>().each(
		[&world, &entities, &events](const auto entity, auto& projectile) {
			// Check for closest intersection
			auto handler = RayCastClosest();
			auto p0 = projectile.origin;
			auto p1 = projectile.origin + projectile.direction * 100.0f;
			world->RayCast(&handler, b2Vec2(p0.x, p0.z), b2Vec2(p1.x, p1.z));
			if (handler.hitEntity()) {
				const auto hitEntity = *handler.hitEntity();
				// Assure entity still exists
				if (entities.valid(hitEntity)) {
					if (entities.has<component::Health>(hitEntity)) {
						events->enqueue<events::ProjectileHit>(hitEntity);
					}
				}
			}
			// Remove projectile
			entities.destroy(entity);
		}
	);
}

void PhysicsSystem::constructPhysics(EntityRegistry& entities, Entity entity) {
	auto& physics = entities.get<component::Physics>(entity);

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(physics.position.x, physics.position.z);

	b2PolygonShape shape;
	shape.SetAsBox(physics.size / 3.0f, physics.size / 3.0f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 1.0f;
	fixtureDef.userData = new Entity(entity);

	physics.body = m_world->CreateBody(&bodyDef);
	physics.fixture = physics.body->CreateFixture(&fixtureDef);
}

void PhysicsSystem::destructPhysics(EntityRegistry& entities, Entity entity) {
	auto& physics = entities.get<component::Physics>(entity);
}