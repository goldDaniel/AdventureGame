#include "systems/RenderSystem.h"
#include "Components.h"

using namespace dg3d::game;

RenderSystem::RenderSystem(entt::registry& registry, graphics::SpriteBatch& sb) 
	: core::GameSystem(registry)
	, mSpriteBatch(sb)
{
}

void RenderSystem::Update(float alpha)
{
	std::vector<entt::entity> toKeep;

	glm::mat4 proj(1.0f);
	glm::mat4 view(1.0f);
	mRegistry.view<const CameraComponent>().each([&proj, &view](const auto& camera)
	{
		proj = camera.proj;
		view = glm::lookAt(glm::vec3{ camera.position, camera.zoom }, { camera.position, 0 }, { 0,1,0 });
	});


	mSpriteBatch.Begin(proj, view);
	mRegistry.view<const PositionComponent, const RenderableComponent>().each([this, &alpha, &toKeep](auto entity, const auto& position, const auto& renderable)
	{
		float width = renderable.width;
		if (!renderable.facingRight) width = -width;

		mSpriteBatch.SetColor(renderable.color);
		mSpriteBatch.Draw(&renderable.texture, position.pos.x, position.pos.y, width, renderable.height);
	});
	mSpriteBatch.End();
}