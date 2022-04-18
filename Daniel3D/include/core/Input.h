#pragma once

#include <SDL.h>

#include <glm/glm.hpp>
#include <functional>
#include <unordered_map>

#include <imgui/imgui_impl_sdl.h>

namespace dg3d
{
	namespace core
	{
        class Input
        {
        public:
            Input(const std::function<void()> quitCallback, const std::function<void(int, int)> resizeCallback)
                : mQuitCallback(quitCallback)
                , mResizeCallback(resizeCallback)
            {
            }

            bool IsKeyDown(SDL_KeyCode code) const
            {
                if (mKeyDown.find(code) != mKeyDown.end())
                {
                    return mKeyDown.at(code);
                }
                return false;
            }

            bool IsButtonDown(int button) const
            {
                return mButtonDown.at(button);
            }

            //relative to top-left
            const glm::vec2 GetMousePos() const
            {
                return { mMouseX, mMouseY };
            }

            void Update()
            {
                SDL_Event event;
                while (SDL_PollEvent(&event))
                {
                    ImGui_ImplSDL2_ProcessEvent(&event);

                    switch (event.type)
                    {
                    case SDL_WINDOWEVENT_RESIZED:
                    case SDL_WINDOWEVENT_SIZE_CHANGED:
						mResizeCallback(event.window.data1, event.window.data2);
                        break;

                    case SDL_QUIT:
                        mQuitCallback();
                        break;

                    case SDL_KEYDOWN:
                        mKeyDown[event.key.keysym.sym] = true;
                        break;
                    case SDL_KEYUP:
                        mKeyDown[event.key.keysym.sym] = false;
                        break;

                    case SDL_MOUSEMOTION:
                        SDL_GetGlobalMouseState(&mMouseX, &mMouseY);
                        break;

                    case SDL_MOUSEBUTTONDOWN:
                        mButtonDown[event.button.button] = true;
                        break;

                    case SDL_MOUSEBUTTONUP:
                        mButtonDown[event.button.button] = false;
                        break;
                    }
                }
            }

        private:

            int mMouseX;
            int mMouseY;

            std::function<void()> mQuitCallback;
            std::function<void(int, int)> mResizeCallback;

            std::unordered_map<int, bool> mButtonDown;

            std::unordered_map<SDL_Keycode, bool> mKeyDown;
        };
	}
}