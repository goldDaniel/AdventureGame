#include "Common.h"

#include "systems/GameSystem.h"

#include "TilemapImporter.h"
#include <imgui/imgui.h>

#include "systems/InputSystem.h"
#include "systems/JumpSystem.h"
#include "systems/MovementSystem.h"
#include "systems/GravitySystem.h"
#include "systems/DirectionSystem.h"
#include "systems/CameraFollowSystem.h"
#include "systems/AnimationSystem.h"

namespace dg3d
{
	class SampleApplication : public core::Application
	{
		std::unique_ptr<graphics::Renderer> mRenderer;
		std::unique_ptr<graphics::SpriteBatch> mSpriteBatch;
		std::unique_ptr<graphics::ShapeRenderer> mShapeRenderer;

		entt::registry mRegistry;

		std::vector<std::unique_ptr<core::GameSystem>> mSystems;



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
		}

		virtual ~SampleApplication()
		{

		}

	protected:

		virtual void Update(float dt) override
		{
			for (auto&& system : mSystems)
			{
				system->Update(dt);
			}
		}

		virtual void Render(float alpha) override
		{
			float aspect = static_cast<float>(mScreenWidth) / mScreenHeight;
			mRegistry.view<CameraComponent>().each([aspect](auto& camera)
			{
				auto proj = glm::perspective(65.0f, aspect, 0.001f, 10.f);
				camera.proj = proj;
			});
			mRenderer->UpdateViewport(mScreenWidth, mScreenHeight);
			mRenderer->Clear();
		
			//background 
			{
				auto bgTex = mRenderer->CreateTexture2D("assets/textures/background.png");
				auto proj = glm::perspective(65.0f, aspect, 0.001f, 10.f);
				auto view = glm::lookAt(glm::vec3{ 0.f, 0.f, 2.f }, { 0.f, 0.f, 0.f }, { 0.f, 1.f, 0.f });


				float scale = bgTex->mWidth / static_cast<float>(bgTex->mHeight);
				float height = 8.f;
				float width = height * scale;

				mSpriteBatch->Begin(proj, view);
				mSpriteBatch->Draw(bgTex, 0.0f, 0.f, width, height);
				mSpriteBatch->End();
			}

			game::RenderSystem::Update(mRegistry, *mSpriteBatch, alpha);
			game::DebugRenderSystem::Update(mRegistry, *mShapeRenderer);
		}
	private:

		template<typename SystemType, class... Dependencies>
		void AddGameSystem(Dependencies&&... deps)
		{
			mSystems.push_back(std::make_unique<SystemType>(mRegistry, deps...));
		}
	};
}