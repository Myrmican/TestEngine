#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_gpu.h>

namespace Engine
{
    class Camera;

    class Renderer {
    public:
        Renderer() = default;
        ~Renderer();

        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;

        bool init(SDL_Window* window);
        void shutdown();
        void renderFrame(const Camera& camera);

        void setWireframeMode(bool useWireframe);

    private:
        SDL_Window* m_window = nullptr;
        SDL_GPUDevice* m_device = nullptr;
        bool wireframeMode = false;

        SDL_GPUGraphicsPipeline* fillPipeline;
        SDL_GPUGraphicsPipeline* linePipeline;
    };
}