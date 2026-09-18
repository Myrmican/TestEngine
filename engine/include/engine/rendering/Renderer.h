#pragma once

// This header is public API for the `engine` module: anything that links
// against `engine` (your `client` targets, `studio`) can
// #include "Rendering/Renderer.hpp" and use this class. That's why it
// lives under include/ rather than src/ — src/ is for implementation
// files nothing outside the module should need to see.

#include <SDL3/SDL.h>
#include <cstdint>

namespace Engine::Rendering
{
    // A simple vertex: position (x, y, z) + color (r, g, b, a).
    // Plain data, laid out exactly how the GPU will read it.
    struct Vertex
    {
        float x, y, z;
        float r, g, b, a;
    };

    // Owns every GPU resource the renderer needs: the device, the
    // pipeline (compiled shaders + draw settings), and buffers.
    //
    // One Renderer is created per window. In your platform, that likely
    // means: `client` creates one Renderer for the game viewport, and
    // `studio` creates one Renderer per open editor viewport (scene view,
    // asset preview, etc.) — each owns its own GPU pipeline and buffers,
    // but they can all share the same SDL_GPUDevice if you want (see the
    // note on `Renderer(SDL_Window*, SDL_GPUDevice*)` below).
    class Renderer
    {
    public:
        // Pass an existing device to share one GPU device across multiple
        // windows/viewports (recommended for studio, where you might have
        // several viewports open at once). Pass nullptr to have this
        // Renderer create and own its own device — simplest option, and
        // what `client` will usually want for its single game window.
        explicit Renderer(SDL_Window* window, SDL_GPUDevice* sharedDevice = nullptr);

        ~Renderer();

        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;

        // Draw one frame: acquire the swapchain image, run a render pass
        // that clears the screen and draws our triangle, then present it.
        void DrawFrame();

        // Exposed so multiple Renderers can share one device if you want
        // that (e.g. studio's several viewport panes).
        SDL_GPUDevice* GetDevice() const { return m_device; }

    private:
        SDL_GPUShader* LoadShader(const char* path,
            SDL_GPUShaderStage stage,
            Uint32 samplerCount,
            Uint32 uniformBufferCount);

        SDL_Window* m_window = nullptr;
        SDL_GPUDevice* m_device = nullptr;
        bool m_ownsDevice = false; // true if we created m_device ourselves

        SDL_GPUGraphicsPipeline* m_pipeline = nullptr;
        SDL_GPUBuffer* m_vertexBuffer = nullptr;
    };
}