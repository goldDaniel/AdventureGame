#include "systems/CameraFollowSystem.h"

#include "Components.h"

using namespace dg3d::game;

CameraFollowSystem::CameraFollowSystem(entt::registry& registry)
	: core::GameSystem(registry)
{
}

void CameraFollowSystem::Update(float dt)
{
	glm::vec2 position{ 0,0 };
	mRegistry.view<const PositionComponent, CameraTargetComponent>().each([&dt, &position](const auto& pos, auto& target)
	{
		target.position = glm::mix(target.position, pos.pos, 2.f * dt);
		position = target.position;
	});

	mRegistry.view<CameraComponent>().each([&position](auto& cam)
	{
		cam.position = position;
	});
}
