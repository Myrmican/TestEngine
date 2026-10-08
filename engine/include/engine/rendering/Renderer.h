#pragma once

#include <SDL3/SDL.h>

namespace Engine
{
    class Renderer
    {
    public:
        Renderer() = default;
        ~Renderer();

        Renderer(const Renderer&) = delete;            // owns GPU resources,
        Renderer& operator=(const Renderer&) = delete; // so no copying

        bool init(SDL_Window* window);
        void shutdown();
        void renderFrame();

    private:
        SDL_Window* m_window = nullptr; // not owned
        SDL_GPUDevice* m_device = nullptr; // owned
    };
}