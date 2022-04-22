#pragma once

#include "core/GameSystem.h"
#include "core/Input.h"

namespace dg3d
{
	namespace game
	{
		class MovementSystem : public core::GameSystem
		{
		public:
			MovementSystem(entt::registry& registry);
			virtual void Update(float dt) override;
		};
	}
}
