#include "Common.h"

#include "TilemapImporter.h"
#include <imgui/imgui.h>

#include "systems/InputSystem.h"
#include "systems/JumpSystem.h"
#include "systems/MovementSystem.h"
#include "systems/GravitySystem.h"
#include "systems/DirectionSystem.h"
#include "systems/CameraFollowSystem.h"
#include "systems/AnimationSystem.h"

#include "systems/BackgroundRenderSystem.h"
#include "systems/RenderSystem.h"
#include "systems/DebugRenderSystem.h"

namespace dg3d
{
	class SampleApplication : public core::Application
	{
		std::unique_ptr<graphics::Renderer> mRenderer;
		std::unique_ptr<graphics::SpriteBatch> mSpriteBatch;
		std::unique_ptr<graphics::ShapeRenderer> mShapeRenderer;

		entt::registry mRegistry;

		std::vector<std::unique_ptr<core::GameSystem>> mGameSystems;

		std::vector<std::unique_ptr<core::GameSystem>> mRenderSystems;


	public:
		SampleApplication(const std::string& title, int w, int h)
			: Application::Application(title, w, h)
		{
			mRenderer = std::make_unique<graphics::Renderer>();
			mShapeRenderer = std::make_unique<graphics::ShapeRenderer>();
			mSpriteBatch = std::make_unique<graphics::SpriteBatch>();

			mRenderer->SetClearColor({ 0.7f, 0.6f, 0.5f, 1.0f });

			mRegistry.emplace<CameraComponent>(mRegistry.create());
			game::tilemap::Create(mRegistry, *mRenderer);


			AddGameSystem<game::InputSystem>(*mInput);
			AddGameSystem<game::JumpSystem>(*mInput);
			AddGameSystem<game::MovementSystem>();
			AddGameSystem<game::GravitySystem>();
			AddGameSystem<game::DirectionSystem>();
			AddGameSystem<game::CameraFollowSystem>();
			AddGameSystem<game::AnimationSystem>();

			AddRenderSystem<game::BackgroundRenderSystem>(*mRenderer, *mSpriteBatch, mScreenWidth, mScreenHeight);
			AddRenderSystem<game::RenderSystem>(*mSpriteBatch);
			AddRenderSystem<game::DebugRenderSystem>(*mShapeRenderer);
		}

		virtual ~SampleApplication()
		{

		}

	protected:

		virtual void Update(float dt) override
		{
			for (auto& system : mGameSystems)
			{
				system->Update(dt);
			}
		}

		virtual void Render(float alpha) override
		{
			mRenderer->UpdateViewport(mScreenWidth, mScreenHeight);
			mRenderer->Clear();

			for (auto& system : mRenderSystems)
			{
				system->Update(alpha);
			}
		}

	private:

		template<typename SystemType, typename... Dependencies>
		void AddGameSystem(Dependencies&&... deps)
		{
			AddSystem<SystemType>(mGameSystems, deps...);
		}

		template<typename SystemType, typename... Dependencies>
		void AddRenderSystem(Dependencies&&... deps)
		{
			AddSystem<SystemType>(mRenderSystems, deps...);
		}

		template<typename SystemType, typename... Dependencies>
		void AddSystem(std::vector< std::unique_ptr<core::GameSystem>>& systems, Dependencies&&... deps)
		{
			systems.push_back(std::make_unique<SystemType>(mRegistry, deps...));
		}
	};
}