#include "systems/GravitySystem.h"

#include "Components.h"

using namespace dg3d::game;

GravitySystem::GravitySystem(entt::registry& registry)
	: core::GameSystem(registry)
{
}

void GravitySystem::Update(float dt)
{
	mRegistry.view<VelocityComponent, const GravityComponent>().each([dt](auto& vel, const auto& gravity)
	{
		vel.velocity.y += gravity.strength * dt;
	});
}
