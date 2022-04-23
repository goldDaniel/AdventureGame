#pragma once

#include "Common.h"

namespace dg3d
{
	struct InputConfigComponent
	{
		SDL_KeyCode left;
		SDL_KeyCode right;
		SDL_KeyCode jump;
	};

	struct TilemapComponent
	{
		//ivec2 is storing the WORLD POSITION for the given tile
		std::unordered_map<glm::ivec2, entt::entity> tileEntities;
	};

	struct JumpComponent
	{
		bool canJump = false;
		float strength = 12.0f;
	};

	struct SideToSideAIComponent 
	{
	};

	struct GravityComponent
	{
		float strength = -24.0f;
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
	
	struct CameraTargetComponent
	{
		glm::vec2 position{ 0,0 };
	};

	struct CameraComponent
	{
		glm::mat4 proj = glm::mat4(1.0f);
		glm::vec2 position{ 0, 0 };
		float zoom = 3;
	};

	struct AnimationContainerComponent
	{
		std::shared_ptr<graphics::Animation> idle;
		std::shared_ptr<graphics::Animation> walk;
	};

	struct AnimationComponent
	{
		std::shared_ptr<graphics::Animation> animation;
	};

	struct RenderableComponent
	{
		graphics::TextureRegion texture;
		glm::vec4 color{ 1,1,1,1 };
		float width = 1.0f;
		float height = 1.0f;

		bool facingRight = true;
	};

	struct DebugRenderableComponent
	{
		glm::vec4 color;
		float width = 1.0f;
		float height = 1.0f;
	};
}
