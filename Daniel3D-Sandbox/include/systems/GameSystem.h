#pragma once

#include "Components.h"

#include <Common.h>

namespace dg3d
{
	namespace game
	{	

		namespace RenderSystem
		{ 
			void Update(entt::registry& registry, graphics::SpriteBatch& sb, float alpha)
			{
				static std::unordered_map<entt::entity, glm::vec2> previousPos;
				std::vector<entt::entity> toKeep;

				glm::mat4 proj(1.0f);
				glm::mat4 view(1.0f);
				registry.view<CameraComponent>().each([&proj, &view](const auto& camera)
				{
					proj = camera.proj;
					view = glm::lookAt(glm::vec3{ camera.position, camera.zoom }, { camera.position, 0 }, { 0,1,0 });
				});
				

				sb.Begin(proj, view);
				registry.view<const PositionComponent, const RenderableComponent>().each([&alpha, &sb, &toKeep](auto entity, const auto& position, const auto& renderable)
				{
					glm::vec2 renderPos{ 0,0 };
					if(previousPos.find(entity) == previousPos.end())
					{
						previousPos[entity] = position.pos;
					}
					glm::vec2 prev = previousPos[entity];
					renderPos = position.pos * alpha + prev * (1.0f - alpha);
					toKeep.push_back(entity);

					sb.SetColor(renderable.color);

					float width = renderable.width;
					if (!renderable.facingRight)
					{
						width = -width;
					}

					sb.Draw(&renderable.texture, renderPos.x, renderPos.y, width, renderable.height);
				});
				sb.End();

				previousPos.clear();
				for (auto entity : toKeep)
				{
					previousPos[entity] = registry.get<PositionComponent>(entity).pos;
				}
			}
		}

		namespace DebugRenderSystem
		{
			void Update(entt::registry& registry, graphics::ShapeRenderer& s)
			{
				CameraComponent cam;
				registry.view<const CameraComponent>().each([&cam](const auto& camera)
					{
						cam = camera;
					});
				auto proj = cam.proj;
				auto viewMat = glm::lookAt(glm::vec3{ cam.position, cam.zoom }, { cam.position, 0 }, { 0,1,0 });
				
				auto view = registry.view<const PositionComponent, const DebugRenderableComponent>();
				s.Begin(proj, viewMat);
				view.each([&s](const auto& position, const auto& renderable)
				{
					glm::vec2 pos0 = position.pos - glm::vec2{renderable.width / 2, renderable.height / 2};
					glm::vec2 pos1 = position.pos + glm::vec2{renderable.width / 2, renderable.height / 2};

					s.SetColor(renderable.color);
					s.Rect(pos0, pos1);
				});
				s.End();
			}
		}
	}
}