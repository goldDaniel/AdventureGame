#pragma once

#include "core/GameSystem.h"
#include "graphics/ShapeRenderer.h"
#include "Components.h"

namespace dg3d
{
	namespace game
	{
		class DebugRenderSystem : public core::GameSystem
		{
		public:
			DebugRenderSystem(entt::registry& registry, graphics::ShapeRenderer& s)
				: core::GameSystem(registry)
				, mShapeRenderer(s)
			{
			}

			virtual void Update(float alpha) override
			{
				CameraComponent cam;
				mRegistry.view<const CameraComponent>().each([&cam](const auto& camera)
					{
						cam = camera;
					});
				auto proj = cam.proj;
				auto viewMat = glm::lookAt(glm::vec3{ cam.position, cam.zoom }, { cam.position, 0 }, { 0,1,0 });

				auto view = mRegistry.view<const PositionComponent, const DebugRenderableComponent>();
				mShapeRenderer.Begin(proj, viewMat);
				view.each([this](const auto& position, const auto& renderable)
				{
					glm::vec2 pos0 = position.pos - glm::vec2{ renderable.width / 2, renderable.height / 2 };
					glm::vec2 pos1 = position.pos + glm::vec2{ renderable.width / 2, renderable.height / 2 };

					mShapeRenderer.SetColor(renderable.color);
					mShapeRenderer.Rect(pos0, pos1);
				});
				mShapeRenderer.End();
			}

		private:
			graphics::ShapeRenderer& mShapeRenderer;
		};
	}
}