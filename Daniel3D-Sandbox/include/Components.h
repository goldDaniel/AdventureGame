#pragma once

#include <SDL.h>
#include <glm/glm.hpp>
#include <graphics/TextureRegion.h>

namespace dg3d
{
	struct InputConfigComponent
	{
		SDL_KeyCode left;
		SDL_KeyCode right;
		SDL_KeyCode up;
		SDL_KeyCode down;
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
	};
}
