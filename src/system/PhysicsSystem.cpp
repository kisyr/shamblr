#include "PhysicsSystem.hpp"
#include "../components.hpp"
#include "../events.hpp"
#include <Box2D/Box2D.h>

using namespace shamblr;

class RayCastClosest : public b2RayCastCallback {
	public:
		RayCastClosest(b2Fixture* source = NULL) :
				m_source(source), m_closestFixture(NULL), m_closestFraction(1.0f) {}

		float32 ReportFixture(
			b2Fixture* fixture,
			const b2Vec2& point,
			const b2Vec2& normal,
			float32 fraction
		) {
			m_closestFixture = fixture;
			m_closestPoint = point;
			m_closestNormal = normal;
			m_closestFraction = fraction;
			if (m_source && m_source == fixture) {
				return -1.0f;
			}
			return fraction;
		}

		b2Fixture* hitFixture() const {
			return m_closestFixture;
		}

		b2Vec2 hitPoint() const {
			return m_closestPoint;
		}

		float32 hitFraction() const {
			return m_closestFraction;
		}

	private:
		b2Fixture* m_source;
		b2Fixture* m_closestFixture;
		b2Vec2 m_closestPoint;
		b2Vec2 m_closestNormal;
		float32 m_closestFraction;
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
	auto world = m_world;

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
#if 1
	entities.view<component::Physics, component::Sight>().each(
		[&world, &entities](const auto entity, auto& physics, auto& sight) {
			// Reset sights
			sight.entities.clear();
			// Check for line of sight against others
			entities.view<component::Physics, component::Player>().each(
				[world, &entities, &entity, &physics, &sight](const auto otherEntity, auto& otherPhysics, auto& otherPlayer) {
					if (entity != otherEntity) {
						auto handler = RayCastClosest(physics.fixture);
						auto p0 = b2Vec2(physics.position.x, physics.position.z);
						auto p1 = b2Vec2(otherPhysics.position.x, otherPhysics.position.z);
						world->RayCast(&handler, p0, p1);
						if (handler.hitFixture() && handler.hitFixture()->GetUserData()) {
							const auto hitEntity = *static_cast<Entity*>(handler.hitFixture()->GetUserData());
							if (hitEntity == otherEntity) {
								sight.entities.push_back(hitEntity);
							}
						}
					}
				}
			);
		}
	);
#endif

	// Process rays
#if 1
	entities.view<component::Ray>().each(
		[&world, &entities, &events](const auto entity, auto& ray) {
			// Check for closest intersection
			auto handler = RayCastClosest();
			const auto length = 100.0f;
			const auto p0 = ray.origin;
			const auto p1 = ray.origin + ray.direction * length;
			world->RayCast(&handler, b2Vec2(p0.x, p0.z), b2Vec2(p1.x, p1.z));
			// Report result
			auto event = events::RayCast{ray.origin, ray.direction, length * handler.hitFraction()};
			// TODO: Handle multiple intersections/entities (for example bullet penetration)
			if (handler.hitFixture() && handler.hitFixture()->GetUserData()) {
				event.entities.push_back(*static_cast<Entity*>(handler.hitFixture()->GetUserData()));
			}
			events->enqueue<events::RayCast>(event);
			// Destroy ray
			entities.destroy(entity);
		}
	);
#endif
#if 0
	entities.view<component::Ray>().each(
		[&world, &entities, &events](const auto entity, auto& ray) {
			// Check for closest intersection
			auto handler = RayCastClosest();
			auto p0 = ray.origin;
			auto p1 = ray.origin + ray.direction * 100.0f;
			world->RayCast(&handler, b2Vec2(p0.x, p0.z), b2Vec2(p1.x, p1.z));
			// Register a hit
			if (handler.hitFixture() && handler.hitFixture()->GetUserData()) {
				const auto hitPoint = ray.origin + (ray.direction * 100.0f * handler.hitFraction());
				const auto hitEntity = *static_cast<Entity*>(handler.hitFixture()->GetUserData());
				entities.assign<component::CastedRay>(entity, ray.origin, ray.direction, hitPoint, hitEntity);
			}
			// Remove ray
			entities.remove<component::Ray>(entity);
		}
	);
#endif
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

	m_world->DestroyBody(physics.body);
}
