#pragma once

#include "Components.h"

#include <entt/entt.hpp>
#include <core/Input.h>
#include <glm/gtc/matrix_transform.hpp>
#include <graphics/SpriteBatch.h>

namespace dg3d
{
	namespace game
	{
		namespace InputSystem
		{
			void Update(float dt, entt::registry& registry, const core::Input& input)
			{
				auto view = registry.view<const InputConfigComponent, VelocityComponent>();

				view.each([&input](const auto& config, auto& vel)
				{
					vel.velocity = { 0,0 };
					if (input.IsKeyDown(config.left))
					{
						vel.velocity.x -= 1;
					}
					if (input.IsKeyDown(config.right))
					{
						vel.velocity.x += 1;
					}
					if (input.IsKeyDown(config.down))
					{
						vel.velocity.y -= 1;
					}
					if (input.IsKeyDown(config.up))
					{
						vel.velocity.y += 1;
					}
					
					if (glm::length(vel.velocity) > 0)
					{
						vel.velocity = glm::normalize(vel.velocity) * 4.0f;
					}
				});
			}
		}

		namespace MovementSystem
		{
			void Update(float dt, entt::registry& registry) 
			{
				auto view = registry.view<PositionComponent, const VelocityComponent>();

				view.each([dt](auto& pos, const auto& vel) 
				{ 
					pos.pos += vel.velocity * dt;
				});
			}
		}

		namespace RenderSystem
		{
			void Update(entt::registry& registry, graphics::SpriteBatch& sb, float aspect)
			{
				auto view = registry.view<const PositionComponent, const RenderableComponent>();

				auto proj = glm::perspective(65.0f, aspect, 0.001f, 10.0f);
				auto viewMat = glm::lookAt(glm::vec3{ 0,0,3 }, { 0,0,0 }, { 0,1,0 });

				sb.Begin(proj, viewMat);
				view.each([&sb](const auto& position, auto& renderable)
				{
					sb.Draw(&renderable.texture, position.pos.x, position.pos.y, 1.0f, 1.0f);
				});
				sb.End();
			}
		}
	}
}