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
			mRenderer->SetClearColor({ 0.2f, 0.4f, 0.3f, 1.0f });

			auto daniel = mRegistry.create();
			InputConfigComponent config =
			{
				SDLK_a,
				SDLK_d,
				SDLK_w,
			};
			mRegistry.emplace<InputConfigComponent>(daniel, config);
			mRegistry.emplace<PositionComponent>(daniel, glm::vec2{ 0, 0 });
			mRegistry.emplace<VelocityComponent>(daniel, glm::vec2{ 0, 0 });
			mRegistry.emplace<JumpComponent>(daniel);
			mRegistry.emplace<GravityComponent>(daniel);
			mRegistry.emplace<TilemapColliderComponent>(daniel);
			mRegistry.emplace<RenderableComponent>(daniel, mRenderer->CreateTexture2D("assets/textures/daniel.png"));

			auto kamilah = mRegistry.create();
			config =
			{
				SDLK_LEFT,
				SDLK_RIGHT,
				SDLK_UP,
			};
			mRegistry.emplace<InputConfigComponent>(kamilah, config);
			mRegistry.emplace<PositionComponent>(kamilah, glm::vec2{ 0, 0 });
			mRegistry.emplace<VelocityComponent>(kamilah, glm::vec2{ 0, 0 });
			mRegistry.emplace<GravityComponent>(kamilah);
			mRegistry.emplace<JumpComponent>(kamilah);
			mRegistry.emplace<TilemapColliderComponent>(kamilah);
			mRegistry.emplace<RenderableComponent>(kamilah, graphics::TextureRegion(mRenderer->CreateTexture2D("assets/textures/kamilah.png")));
			mRegistry.emplace<DebugRenderableComponent>(kamilah, glm::vec4(1,1,1,1));

			game::tilemap::Create(mRegistry, [this]() {
				return mRenderer->CreateTexture2D("debug");
			});
		}

		virtual ~SampleApplication()
		{

		}
		
	protected:

		virtual void Update(float dt) override
		{
			ImGui::ShowDemoWindow();

			if (dt > 1.0f / 30.0f) dt = 1.0f / 30.0f;

			game::InputSystem::Update(dt, mRegistry, *input);
			game::JumpSystem::Update(dt, mRegistry, *input);
			game::MovementSystem::Update(dt, mRegistry);

			game::GravitySystem::Update(dt, mRegistry);
			game::DirectionSystem::Update(dt, mRegistry);
		}

		virtual void Render() override
		{
			mRenderer->UpdateViewport(mScreenWidth, mScreenHeight);
			mRenderer->Clear();

			float aspect = mScreenWidth / static_cast<float>(mScreenHeight);
			game::RenderSystem::Update(mRegistry, *mSpriteBatch, aspect);
			game::DebugRenderSystem::Update(mRegistry, *mShapeRenderer, aspect);
		}
	};
}