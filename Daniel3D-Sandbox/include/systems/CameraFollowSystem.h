#pragma once

#include "core/GameSystem.h"

namespace dg3d
{
	namespace game
	{
		class CameraFollowSystem : public core::GameSystem
		{
		public:
			CameraFollowSystem(entt::registry& registry);
			virtual void Update(float dt) override;
		};
	}
}
