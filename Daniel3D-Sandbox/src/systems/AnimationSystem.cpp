#include "systems/AnimationSystem.h"

#include "Components.h"

using namespace dg3d::game;

AnimationSystem::AnimationSystem(entt::registry& registry)
	: core::GameSystem(registry)
{
}

void AnimationSystem::Update(float dt)
{
	mRegistry.view<AnimationComponent>().each([this, &dt](auto entity, auto& anim)
	{
		anim.animation.Update(dt);
		if (auto renderable = mRegistry.try_get<RenderableComponent>(entity))
		{
			renderable->texture = anim.animation.GetCurrentFrame();
		}
	});
}
