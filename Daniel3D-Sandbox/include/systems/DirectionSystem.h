#pragma once

#include "core/GameSystem.h"

namespace dg3d
{
	namespace game
	{
		class DirectionSystem : public core::GameSystem
		{
		public:
			DirectionSystem(entt::registry& registry);
			virtual void Update(float dt) override;
		};
	}
}
