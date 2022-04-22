#pragma once

#include "core/GameSystem.h"
#include "core/Input.h"

namespace dg3d
{
	namespace game
	{
		class JumpSystem : public core::GameSystem
		{
		public:
			JumpSystem(entt::registry& registry, const core::Input& input);

			virtual void Update(float dt) override;

		private:
			const core::Input& mInput;
		};
	}
}
