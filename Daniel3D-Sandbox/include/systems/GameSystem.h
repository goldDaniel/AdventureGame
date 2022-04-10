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
					vel.velocity.x = 0;
					float speed = 4;
					if (input.IsKeyDown(config.left))
					{
						vel.velocity.x -= speed;
					}
					if (input.IsKeyDown(config.right))
					{
						vel.velocity.x += speed;
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

			bool ResolveCollision(const entt::entity& player, PositionComponent& playerPos, VelocityComponent& vel, const TilemapColliderComponent& collider, const glm::vec2& tileMin, const glm::vec2& tileMax, entt::registry& registry, bool horizontal)
			{
				glm::vec2 diff = tileMax - tileMin;
				glm::vec4 tileRect = { tileMin + diff / 2.0f, diff };
				glm::vec4 playerRect = { playerPos.pos, collider.width, collider.height };

				CollisionSide side = getCollisionSide(tileRect, playerRect);

				if (horizontal)
				{
					if (side == CollisionSide::Right)
					{
						playerPos.pos.x = tileMin.x - collider.width / 2;
						return true;
					}
					if (side == CollisionSide::Left)
					{
						playerPos.pos.x = tileMax.x + collider.width / 2;
						return true;
					}
				}
				else
				{
					if (side == CollisionSide::Top)
					{
						playerPos.pos.y = tileMin.y - collider.height / 2;
						vel.velocity.y = 0;
						return true;
					}
					if (side == CollisionSide::Bottom)
					{
						playerPos.pos.y = tileMax.y + collider.height / 2;
						vel.velocity.y = 0;
						registry.emplace_or_replace<JumpComponent>(player);
						return true;
					}
				}

				return false;
			}
			
			bool CheckForTilemapCollisions(const entt::entity& player, PositionComponent& playerPos, VelocityComponent& vel, const TilemapColliderComponent& collider, entt::registry& registry, bool horizontal)
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
							if (ResolveCollision(player, playerPos, vel, collider, min, max, registry, horizontal))
							{
								return true;
							}
						}
						else
						{
							render.color = { 0, 0, 0, 1 };
						}
					}
				}
				return false;
			}

			void Update(float dt, entt::registry& registry, bool horizontal)
			{
				auto view = registry.view < PositionComponent, VelocityComponent, const TilemapColliderComponent > ();
				view.each([&registry, horizontal](auto entity, auto& pos, auto& vel, const auto& collider)
				{
					if (!CheckForTilemapCollisions(entity, pos, vel, collider, registry, horizontal))
					{
						if (!horizontal)
						{
							registry.remove<JumpComponent>(entity);
						}
					}
				});
			}
		}

		namespace JumpSystem
		{
			void Update(float dt, entt::registry& registry, const core::Input& input)
			{
				auto view = registry.view<VelocityComponent, const InputConfigComponent, const JumpComponent>();
				view.each([&registry, &input](auto entity, auto& vel, const auto& config, const auto& jump)
				{
					if (input.IsKeyDown(config.jump))
					{
						vel.velocity.y = jump.strength;
						registry.erase<JumpComponent>(entity);
					}
				});
			}
		}

		namespace GravitySystem
		{
			void Update(float dt, entt::registry& registry)
			{
				auto view = registry.view<VelocityComponent, const GravityComponent>();
				view.each([dt](auto& vel, const auto& gravity)
				{
					vel.velocity.y += gravity.strength * dt;
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
					pos.pos.x += vel.velocity.x * dt;
				});
				game::TilemapCollisionSystem::Update(dt, registry, true);

				view = registry.view<PositionComponent, const VelocityComponent>();
				view.each([dt](auto& pos, const auto& vel)
				{
					pos.pos.y += vel.velocity.y * dt;
				});
				game::TilemapCollisionSystem::Update(dt, registry, false);
					
			}
		}

		namespace DirectionSystem
		{
			void Update(float dt, entt::registry& registry)
			{
				auto view = registry.view<const VelocityComponent, RenderableComponent>();
				view.each([dt](const auto& vel, auto& renderable)
				{
					if (vel.velocity.x < 0) renderable.facingRight = false;
					if (vel.velocity.x > 0) renderable.facingRight = true;
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

					float width = renderable.width;
					if (!renderable.facingRight)
					{
						width = -width;
					}

					sb.Draw(&renderable.texture, position.pos.x, position.pos.y, width, renderable.height);
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