#pragma once 

#include "core/GameSystem.h"
#include "graphics/SpriteBatch.h"

namespace dg3d
{
	namespace game
	{
		class RenderSystem : public core::GameSystem
		{
		public:
			RenderSystem(entt::registry& registry, graphics::SpriteBatch& sb);
			virtual void Update(float alpha) override;

		private:
			graphics::SpriteBatch& mSpriteBatch;
		};
	}
}