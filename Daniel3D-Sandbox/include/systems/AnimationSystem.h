#pragma once

#include "core/GameSystem.h"

namespace dg3d
{
	namespace game
	{
		class AnimationSystem : public core::GameSystem
		{
		public:
			AnimationSystem(entt::registry& registry);
			virtual void Update(float dt) override;
		};
	}
}
