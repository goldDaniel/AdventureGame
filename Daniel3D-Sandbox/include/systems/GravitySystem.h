#pragma once

#include "core/GameSystem.h"
#include "core/Input.h"

namespace dg3d
{
	namespace game
	{
		class GravitySystem : public core::GameSystem
		{
		public:
			GravitySystem(entt::registry& registry);
			virtual void Update(float dt) override;
		};
	}
}
