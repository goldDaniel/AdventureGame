#pragma once

#include "Common.h"

namespace dg3d
{
	struct InputConfigComponent
	{
		SDL_KeyCode left;
		SDL_KeyCode right;
		SDL_KeyCode up;
		SDL_KeyCode down;
	};

	struct TilemapComponent
	{
		//ivec2 is storing the WORLD POSITION for the given tile
		std::unordered_map<glm::ivec2, entt::entity> tileEntities;
	};

	struct TilemapColliderComponent
	{
		float width =  1.0f;
		float height = 1.0f;
	};

	struct TilemapTileComponent
	{
		bool isSolid = false;
	};

	struct PositionComponent
	{
		glm::vec2 pos{ 0,0 };
	};

	struct VelocityComponent
	{
		glm::vec2 velocity{ 0,0 };
	};

	struct RenderableComponent
	{
		graphics::TextureRegion texture;
		glm::vec4 color{ 1,1,1,1 };
		float width = 1.0f;
		float height = 1.0f;
	};

	struct DebugRenderableComponent
	{
		glm::vec4 color;
		float width = 1.0f;
		float height = 1.0f;
	};
}
