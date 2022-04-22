#include "systems/JumpSystem.h"

#include "Components.h"

using namespace dg3d::game;

JumpSystem::JumpSystem(entt::registry& registry, const core::Input& input)
	: core::GameSystem(registry)
	, mInput(input)
{
}

void JumpSystem::Update(float dt)
{
	mRegistry.view<VelocityComponent, const InputConfigComponent, JumpComponent>().each([this](auto& vel, const auto& config, auto& jump)
	{
		if (mInput.IsKeyDown(config.jump) && jump.canJump)
		{
			vel.velocity.y = jump.strength;
			jump.canJump = false;
		}
	});
}
