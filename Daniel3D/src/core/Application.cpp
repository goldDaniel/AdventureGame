#include "core/Application.h"

#include <graphics/Renderer.h>
#include <iostream>
#include <glad/glad.h>

using namespace dg3d::core;

void Application::InitializeWindow(const std::string& title, int windowW, int windowH)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        std::cerr << "SDL2 video subsystem couldn't be initialized. Error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    SDL_GL_LoadLibrary(NULL);

    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    mWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowW, windowH, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    SDL_SetWindowResizable(mWindow, (SDL_bool)true);

    mGLContext = SDL_GL_CreateContext(mWindow);
    if (!mGLContext || !gladLoadGLLoader(SDL_GL_GetProcAddress))
    {
        std::cerr << "Failed to initialize the OpenGL context." << std::endl;
        exit(1);
    }

    std::cout 
        << "OpenGL loaded\n"
        << "Vendor:   " << glGetString(GL_VENDOR) << "\n"
        << "Renderer: " << glGetString(GL_RENDERER) << "\n"
        << "Version:  " << glGetString(GL_VERSION) << "\n"
        << std::endl;
}

Application::Application(const std::string& title, int width, int height)
    : mTitle(title)
    , mScreenWidth(width)
    , mScreenHeight(height)
    , mRunning(false)
{
    InitializeWindow(title, width, height);
    input = std::make_unique<Input>([this]() 
    { 
        mRunning = false; 
    });
}

Application::~Application()
{
    SDL_GL_DeleteContext(mGLContext);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}

void Application::Run()
{
    mRunning = true;
    uint32_t prevTime = SDL_GetTicks();

    while (mRunning)
    {
        uint32_t currTime = SDL_GetTicks();
        float dt = static_cast<float>(currTime - prevTime) / 1000.f;
        prevTime = currTime;

        SDL_GetWindowSize(mWindow, &mScreenWidth, &mScreenHeight);

        input->Update();
        Update(dt);        
        Render();

        SDL_GL_SwapWindow(mWindow);
    }
}