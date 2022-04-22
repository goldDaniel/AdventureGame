#include "systems/DirectionSystem.h"

#include "Components.h"

using namespace dg3d::game;

DirectionSystem::DirectionSystem(entt::registry& registry)
	: core::GameSystem(registry)
{
}

void DirectionSystem::Update(float dt)
{
	mRegistry.view<const VelocityComponent, RenderableComponent>().each([dt](const auto& vel, auto& renderable)
	{
		if (vel.velocity.x < 0) renderable.facingRight = false;
		if (vel.velocity.x > 0) renderable.facingRight = true;
	});
}
