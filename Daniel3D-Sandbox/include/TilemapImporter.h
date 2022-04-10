#pragma once 

#include "Common.h"

#include <Components.h>

namespace dg3d
{
	namespace game
	{
		namespace tilemap
		{
			std::unordered_map<glm::ivec2, entt::entity> CreateTiles(entt::registry& registry, std::function<graphics::Texture* (void)> isSolidTextureCallback)
			{
				std::unordered_map<glm::ivec2, entt::entity> entities;

				int numLines = 8;// must be equal to (number of lines in string - 1)
				std::string map =
					"xxxxxxxxxxxxxxxxxxx\n"
					"x.................x\n"
					"x..xxx.....xxx....x\n"
					"x....x.....x.....x\n"
					"x....xx......x....x\n"
					"x....x.......xx...x\n"
					"x....xxxxxx.......x\n"
					"x.................x\n"
					"xxxxxxxxxxxxxxxxxxx";

				std::istringstream stream(map);
				std::string line;

				float y = numLines / 2.0f;
				while (std::getline(stream, line))
				{
					for (size_t i = 0; i < line.length(); ++i)
					{
						glm::ivec2 pos = { i - line.length() / 2.0f, y };
						auto entity = registry.create();
						entities[pos] = entity;

						registry.emplace<PositionComponent>(entity, glm::vec2{ pos.x, pos.y } );

						bool isSolid = false;
						if (line[i] == 'x')
						{
							isSolid = true;
							registry.emplace<RenderableComponent>(entity, graphics::TextureRegion(isSolidTextureCallback()), glm::vec4{ 0.3, 0.2f, 0.5f, 1.0f });
							registry.emplace<DebugRenderableComponent>(entity,
								glm::vec4{ 0, 0, 0, 1.0f });
						}
						if (line[i] == '.')
						{
							isSolid = false;
							registry.emplace<DebugRenderableComponent>(entity,
								glm::vec4{ 0.5, 0.3f, 0.8f, 1.0f });
						}



						registry.emplace<TilemapTileComponent>(entity, isSolid);
					}
					y--;
				}
				return entities;
			}

			void Create(entt::registry& registry, std::function<graphics::Texture*(void)> isSolidTextureCallback)
			{
				auto tiles = CreateTiles(registry, isSolidTextureCallback);

				auto tilemapEntity = registry.create();
				TilemapComponent tilemap = { tiles };
				registry.emplace<TilemapComponent>(tilemapEntity, tilemap);
			}

			
		}
	}
}