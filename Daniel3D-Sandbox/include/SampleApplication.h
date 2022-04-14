#include "Common.h"

#include "systems/GameSystem.h"

#include "TilemapImporter.h"
#include <imgui/imgui.h>

namespace dg3d
{
	class SampleApplication : public core::Application
	{
		std::unique_ptr<graphics::Renderer> mRenderer;
		std::unique_ptr<graphics::SpriteBatch> mSpriteBatch;
		std::unique_ptr<graphics::ShapeRenderer> mShapeRenderer;
		
		entt::registry mRegistry;

	public:
		SampleApplication(const std::string& title, int w, int h)
			: Application::Application(title, w, h) 
		{
			mRenderer = std::make_unique<graphics::Renderer>();
			mShapeRenderer  = std::make_unique<graphics::ShapeRenderer>();
			mSpriteBatch = std::make_unique<graphics::SpriteBatch>();
			mRenderer->SetClearColor({ 0.7f, 0.6f, 0.5f, 1.0f });
			
			auto cam = mRegistry.create();
			mRegistry.emplace<CameraComponent>(cam);


			game::tilemap::Create(mRegistry, *mRenderer);
		}

		virtual ~SampleApplication()
		{

		} 
		
	protected:

		virtual void Update(float dt) override
		{
			game::InputSystem::Update(dt, mRegistry, *input);
			game::JumpSystem::Update(dt, mRegistry, *input);
			game::MovementSystem::Update(dt, mRegistry);

			game::GravitySystem::Update(dt, mRegistry);
			game::DirectionSystem::Update(dt, mRegistry);

			game::CameraSystem::Update(mRegistry, dt, *input);
		}

		virtual void Render(float alpha) override
		{
			mRenderer->UpdateViewport(mScreenWidth, mScreenHeight);
			mRenderer->Clear();

			mSpriteBatch->Begin(glm::mat4(1.0f), glm::mat4(1.0f));
			mSpriteBatch->Draw(mRenderer->CreateTexture2D("assets/textures/background.png"), 0, 0, 2, 2);
			mSpriteBatch->End();

			float aspect = mScreenWidth / static_cast<float>(mScreenHeight);
			game::RenderSystem::Update(mRegistry, *mSpriteBatch, aspect, alpha);
			game::DebugRenderSystem::Update(mRegistry, *mShapeRenderer, aspect);

			
		}
	};
}