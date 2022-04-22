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
			glm::vec2 renderPos{ 0,0 };
			if (mPrevPos.find(entity) == mPrevPos.end())
			{
				mPrevPos[entity] = position.pos;
			}
			glm::vec2 prev = mPrevPos[entity];

			renderPos = position.pos * alpha + prev * (1.0f - alpha);
			toKeep.push_back(entity);


			float width = renderable.width;
			if (!renderable.facingRight) width = -width;

			mSpriteBatch.SetColor(renderable.color);
			mSpriteBatch.Draw(&renderable.texture, renderPos.x, renderPos.y, width, renderable.height);
		});
	mSpriteBatch.End();

	mPrevPos.clear();
	for (auto entity : toKeep)
	{
		mPrevPos[entity] = mRegistry.get<PositionComponent>(entity).pos;
	}
}