#pragma once 

#include <entt/entt.hpp>

namespace dg3d
{
	namespace core
	{
		class GameSystem
		{
		public:
			GameSystem(entt::registry& registry)
				: mRegistry(registry)
			{
			}

			GameSystem() = delete;
			GameSystem(const GameSystem& sys) = delete;
			GameSystem(GameSystem&& sys) = delete;

			virtual void Update(float dt) = 0;

		protected:
			entt::registry& mRegistry;
		};
	}
}