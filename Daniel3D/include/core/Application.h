#pragma once

#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <string>
#include <memory>
#include <core/Input.h>
#include <graphics/Renderer.h>

namespace dg3d
{
	namespace core
	{
		class Application
		{
		private:
			SDL_Window* mWindow;
			SDL_GLContext mGLContext;

			float mTime;
			float mAccumulator;

			bool mRunning;

		protected:
			std::unique_ptr<Input> mInput;

			std::string mTitle;
			int mScreenWidth;
			int mScreenHeight;

		public:
			Application(const std::string& title, int width, int height);
			~Application();

			void Run();

		protected:
			virtual void Update(float delta) = 0;
			virtual void Render(float alpha) = 0;

			virtual void OnResize(int width, int height) {}

		private:
			void InitializeWindow(const std::string& title, int width, int height);
		};
	}
}