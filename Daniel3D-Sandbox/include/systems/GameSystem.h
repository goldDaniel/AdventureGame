#pragma once

#include "Components.h"

#include <Common.h>

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

		
		namespace TilemapCollisionSystem
		{
			enum class CollisionSide
			{
				INVALID,

				Left,
				Right,
				Top,
				Bottom
			}; 

			CollisionSide getCollisionSide(const glm::vec4& r0, const glm::vec4& r1)
			{
				CollisionSide result = CollisionSide::INVALID;
				//horizontal side
				bool left = r0.x + r0.z / 2 < r1.x + r1.z / 2;
				//vertical side
				bool above = r0.y + r0.w / 2 > r1.y + r1.w / 2;

				//holds how deep the r1ect is inside the tile on each axis
				float horizontalDif = 0;
				float verticalDif = 0;

				//determine the differences for depth
				if (left)
				{
					horizontalDif = r0.x + r0.z - r1.x;
				}
				else
				{
					horizontalDif = r1.x + r1.z - r0.x;
				}

				if (above)
				{
					verticalDif = r1.y + r1.w - r0.y;
				}
				else
				{
					verticalDif = r0.y + r0.w - r1.y;
				}

				if (horizontalDif < verticalDif)
				{
					if (left)
					{
						result = CollisionSide::Left;
					}
					else
					{
						result = CollisionSide::Right;
					}
				}
				else if (above)
				{
					result = CollisionSide::Top;
				}
				else
				{
					result = CollisionSide::Bottom;
				}

				return result;
			}

			void ResolveCollision(PositionComponent& playerPos, const TilemapColliderComponent& collider, const const glm::vec2& tileMin, const glm::vec2& tileMax)
			{
				glm::vec2 diff = tileMax - tileMin;
				glm::vec4 tileRect = { tileMin + diff / 2.0f, diff };
				glm::vec4 playerRect = { playerPos.pos, collider.width, collider.height };

				CollisionSide side = getCollisionSide(tileRect, playerRect);

				if (side == CollisionSide::Top)
				{
					playerPos.pos.y = tileMin.y - collider.height / 2;
				}
				if (side == CollisionSide::Bottom)
				{
					playerPos.pos.y = tileMax.y + collider.height / 2;
				}
				if (side == CollisionSide::Right)
				{
					playerPos.pos.x = tileMin.x - collider.width / 2;
				}
				if (side == CollisionSide::Left)
				{
					playerPos.pos.x = tileMax.x + collider.width / 2;
				}
			}
			
			void CheckForTilemapCollisions(PositionComponent& playerPos, const TilemapColliderComponent& collider, entt::registry& registry)
			{
				std::vector<entt::entity> collisionTiles;
				//gather collision tiles
				auto view = registry.view<TilemapComponent>();
				view.each([&playerPos, &collider, &collisionTiles](auto& tilemap)
				{
					glm::ivec2 keyBase { glm::round(playerPos.pos.x), glm::round(playerPos.pos.y) };
					for (int y = -1; y <= 1; ++y)
					{
						for (int x = -1; x <= 1; ++x)
						{
							glm::ivec2 key = keyBase + glm::ivec2(x, y);
							if (tilemap.tileEntities.find(key) != tilemap.tileEntities.end())
							{
								auto tile = tilemap.tileEntities[key];
								collisionTiles.push_back(tile);
							}
						}
					}
				});

				for(auto entity : collisionTiles)
				{
					auto& position = registry.get<PositionComponent>(entity);
					auto& tile = registry.get<TilemapTileComponent>(entity);
					auto& render = registry.get<DebugRenderableComponent>(entity);
					if (tile.isSolid)
					{
						glm::vec2 min = { position.pos.x - 0.5f, position.pos.y - 0.5f };
						glm::vec2 max = { position.pos.x + 0.5f, position.pos.y + 0.5f };
					
						glm::vec2 playerMin = { playerPos.pos.x - 0.5f, playerPos.pos.y - 0.5f };
						glm::vec2 playerMax = { playerPos.pos.x + 0.5f, playerPos.pos.y + 0.5f };

						if ((playerMin.x >= min.x && playerMin.x <= max.x ||
							 playerMax.x >= min.x && playerMax.x <= max.x)
							&&
							(playerMin.y >= min.y && playerMin.y <= max.y ||
							 playerMax.y >= min.y && playerMax.y <= max.y))
						{
							
							render.color = { 1,0,0,1 };
							ResolveCollision(playerPos, collider, min, max);
						}
						else
						{
							render.color = { 0, 0, 0, 1 };
						}
					}
				}
			}

			void Update(float dt, entt::registry& registry)
			{
				auto view = registry.view<PositionComponent, const TilemapColliderComponent>();
				view.each([&registry](auto& pos, const auto& collider)
				{
					CheckForTilemapCollisions(pos, collider, registry);
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
				view.each([&sb](const auto& position, const auto& renderable)
				{
					sb.SetColor(renderable.color);
					sb.Draw(&renderable.texture, position.pos.x, position.pos.y, renderable.width, renderable.height);
				});
				sb.End();
			}
		}

		namespace DebugRenderSystem
		{
			void Update(entt::registry& registry, graphics::ShapeRenderer& s, float aspect)
			{
				auto view = registry.view<const PositionComponent, const DebugRenderableComponent>();

				auto proj = glm::perspective(65.0f, aspect, 0.001f, 10.0f);
				auto viewMat = glm::lookAt(glm::vec3{ 0,0,3 }, { 0,0,0 }, { 0,1,0 });

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