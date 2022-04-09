#include <core/Application.h>

#include <graphics/SpriteBatch.h>
#include <graphics/Renderer.h>

#include "systems/GameSystem.h"

namespace dg3d
{
	class SampleApplication : public core::Application
	{
		std::unique_ptr<graphics::Renderer> mRenderer;
		std::unique_ptr<graphics::SpriteBatch> mSpriteBatch;
		entt::registry mRegistry;

	public:
		SampleApplication(const std::string& title, int w, int h)
			: Application::Application(title, w, h) 
		{
			mRenderer = std::make_unique<graphics::Renderer>();
			mSpriteBatch = std::make_unique<graphics::SpriteBatch>();
			mRenderer->SetClearColor({ 0.2f, 0.4f, 0.3f, 1.0f });

			auto daniel = mRegistry.create();
			InputConfigComponent config =
			{
				SDLK_a,
				SDLK_d,
				SDLK_w,
				SDLK_s,
			};
			mRegistry.emplace<InputConfigComponent>(daniel, config);
			mRegistry.emplace<PositionComponent>(daniel, glm::vec2{ 0, 0 });
			mRegistry.emplace<VelocityComponent>(daniel, glm::vec2{ 0, 0 });
			mRegistry.emplace<RenderableComponent>(daniel, mRenderer->CreateTexture2D("assets/textures/daniel.png"));

			auto kamilah = mRegistry.create();
			config =
			{
				SDLK_LEFT,
				SDLK_RIGHT,
				SDLK_UP,
				SDLK_DOWN,
			};
			mRegistry.emplace<InputConfigComponent>(kamilah, config);
			mRegistry.emplace<PositionComponent>(kamilah, glm::vec2{ 0, 0 });
			mRegistry.emplace<VelocityComponent>(kamilah, glm::vec2{ 0, 0 });
			mRegistry.emplace<RenderableComponent>(kamilah, graphics::TextureRegion(mRenderer->CreateTexture2D("assets/textures/kamilah.png")));
		}

		virtual ~SampleApplication()
		{

		}
		
	protected:

		virtual void Update(float dt) override
		{
			game::InputSystem::Update(dt, mRegistry, *input);
			game::MovementSystem::Update(dt, mRegistry);
		}

		virtual void Render() override
		{
			mRenderer->UpdateViewport(mScreenWidth, mScreenHeight);
			mRenderer->Clear();

			float aspect = mScreenWidth / static_cast<float>(mScreenHeight);
			game::RenderSystem::Update(mRegistry, *mSpriteBatch, aspect);
		}
	};
}