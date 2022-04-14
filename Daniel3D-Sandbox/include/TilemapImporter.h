#pragma once 

#include "Common.h"

#include <Components.h>

namespace dg3d
{
	namespace game
	{
		namespace tilemap
		{
			std::unordered_map<glm::ivec2, entt::entity> CreateTiles(entt::registry& registry, graphics::Renderer& renderer)
			{
				std::unordered_map<glm::ivec2, entt::entity> entities;

				int numLines = 8;// must be equal to (number of lines in string - 1)
				std::string map =
					"................................................x\n"
					"................................................x\n"
					".dk.............................................x\n"
					"....................xxxx........................x\n"
					".......................x...b....................x\n"
					".......................xxxxxxx..................x\n"
					"...............xxx.....x......x.................x\n"
					".......................x.......x................x\n"
					".......................x.c......x...............x\n"
					"..........xxx..........xxxx......x......f.......x\n"
					".......................x...............xxx......x\n"
					".......................x........................x\n"
					"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";

				std::istringstream stream(map);
				std::string line;

				float y = numLines / 2.0f;
				while (std::getline(stream, line))
				{
					for (size_t i = 0; i < line.length(); ++i)
					{
						glm::ivec2 pos = { i - line.length() / 2.0f, y };
						

						if (line[i] == 'x')
						{
							entities[pos] = registry.create();
							registry.emplace<PositionComponent>(entities[pos], glm::vec2{ pos.x, pos.y });
							bool isSolid = true;
							registry.emplace<RenderableComponent>(entities[pos], graphics::TextureRegion(renderer.CreateTexture2D("assets/textures/tile_grass.png")));
							registry.emplace<TilemapTileComponent>(entities[pos], isSolid);
						}
						if (line[i] == 'b')
						{
							auto blob = registry.create();
							registry.emplace<PositionComponent>(blob, pos);
							registry.emplace<VelocityComponent>(blob, glm::vec2{ 0, 0 });
							registry.emplace<GravityComponent>(blob);
							registry.emplace<TilemapColliderComponent>(blob);
							registry.emplace<RenderableComponent>(blob, graphics::TextureRegion(renderer.CreateTexture2D("assets/textures/blob2.png")));
						}
						if (line[i] == 'f')
						{
							auto fire = registry.create();
							registry.emplace<PositionComponent>(fire, pos);
							registry.emplace<RenderableComponent>(fire, graphics::TextureRegion(renderer.CreateTexture2D("assets/textures/fire1.png")));
						}
						if (line[i] == 'c')
						{
							auto chest = registry.create();
							registry.emplace<PositionComponent>(chest, pos);
							registry.emplace<RenderableComponent>(chest, graphics::TextureRegion(renderer.CreateTexture2D("assets/textures/chest1.png")));
						}
						if (line[i] == 'd')
						{
							auto daniel = registry.create();
							InputConfigComponent config =
							{
								SDLK_a,
								SDLK_d,
								SDLK_w,
							};
							registry.emplace<InputConfigComponent>(daniel, config);
							registry.emplace<PositionComponent>(daniel, glm::vec2{ 0, 0 });
							registry.emplace<VelocityComponent>(daniel, glm::vec2{ 0, 0 });
							registry.emplace<JumpComponent>(daniel);
							registry.emplace<GravityComponent>(daniel);
							registry.emplace<TilemapColliderComponent>(daniel);
							registry.emplace<RenderableComponent>(daniel, graphics::TextureRegion(renderer.CreateTexture2D("assets/textures/daniel.png")));
						}
						if (line[i] == 'k')
						{
							auto kamilah = registry.create();
							InputConfigComponent config =
							{
								SDLK_LEFT,
								SDLK_RIGHT,
								SDLK_UP,
							};
							registry.emplace<InputConfigComponent>(kamilah, config);
							registry.emplace<PositionComponent>(kamilah, glm::vec2{ 0, 0 });
							registry.emplace<VelocityComponent>(kamilah, glm::vec2{ 0, 0 });
							registry.emplace<GravityComponent>(kamilah);
							registry.emplace<JumpComponent>(kamilah);
							registry.emplace<TilemapColliderComponent>(kamilah);
							registry.emplace<RenderableComponent>(kamilah, graphics::TextureRegion(renderer.CreateTexture2D("assets/textures/kamilah.png")));
						}
					}
					y--;
				}
				return entities;
			}

			void Create(entt::registry& registry, graphics::Renderer& renderer)
			{
				auto tiles = CreateTiles(registry, renderer);

				auto tilemapEntity = registry.create();
				TilemapComponent tilemap = { tiles };
				registry.emplace<TilemapComponent>(tilemapEntity, tilemap);
			}

			
		}
	}
}