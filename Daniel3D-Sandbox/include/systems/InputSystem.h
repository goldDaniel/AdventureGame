#pragma once

#include "core/GameSystem.h"
#include "core/Input.h"

namespace dg3d
{
	namespace game
	{
		class InputSystem : public core::GameSystem
		{
		public:
			InputSystem(entt::registry& registry, const core::Input& input);

			virtual void Update(float dt) override;

		private:
			const core::Input& mInput;
		};
	}
}
