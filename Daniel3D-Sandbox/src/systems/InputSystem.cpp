#include "systems/InputSystem.h"

#include "Components.h"

using namespace dg3d::game;

InputSystem::InputSystem(entt::registry& registry, const core::Input& input)
		: core::GameSystem(registry)
		, mInput(input)
{
}

void InputSystem::Update(float dt)
{
	mRegistry.view<const InputConfigComponent, VelocityComponent>().each([this](const auto& config, auto& vel)
	{
		vel.velocity.x = 0;
		float speed = 4;
		if (mInput.IsKeyDown(config.left))
		{
			vel.velocity.x -= speed;
		}
		if (mInput.IsKeyDown(config.right))
		{
			vel.velocity.x += speed;
		}
	});
}
