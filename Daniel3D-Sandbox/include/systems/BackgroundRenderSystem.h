#pragma once 

#include "core/GameSystem.h"
#include "graphics/SpriteBatch.h"
#include "graphics/Renderer.h"
#include "Components.h"

namespace dg3d
{
	namespace game
	{
		class BackgroundRenderSystem : public core::GameSystem
		{
		public:
			BackgroundRenderSystem(entt::registry& registry, graphics::Renderer& renderer, graphics::SpriteBatch& sb, int& w, int& h)
				: core::GameSystem(registry)
				, mRenderer(renderer)
				, mSpriteBatch(sb)
				, mScreenWidth(w)
				, mScreenHeight(h)
			{
			}

			virtual void Update(float alpha) override
			{
				float aspect = static_cast<float>(mScreenWidth) / mScreenHeight;
				
				auto bgTex = mRenderer.CreateTexture2D("assets/textures/background.png");

				auto proj = glm::perspective(65.0f, aspect, 0.001f, 10.f);
				auto view = glm::lookAt(glm::vec3{ 0.f, 0.f, 2.f }, { 0.f, 0.f, 0.f }, { 0.f, 1.f, 0.f });
				mRegistry.view<CameraComponent>().each([aspect](auto& camera)
				{
					auto proj = glm::perspective(65.0f, aspect, 0.001f, 10.f);
					camera.proj = proj;
				});

				float scale = bgTex->mWidth / static_cast<float>(bgTex->mHeight);
				float height = 8.f;
				float width = height * scale;

				mSpriteBatch.Begin(proj, view);

				for (int i = -1; i <= 1; ++i)
				{
					mSpriteBatch.Draw(bgTex, i * width / 2.0f, 0.f, width, height);
				}
				mSpriteBatch.End();
			}

		private:

			graphics::SpriteBatch& mSpriteBatch;
			graphics::Renderer& mRenderer;

			const int& mScreenWidth;
			const int& mScreenHeight;
		};
	}
}