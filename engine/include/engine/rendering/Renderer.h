#pragma once

// engine/include/engine/Rendering/Renderer.hpp
//
// Public header — consumers (client, studio) include this as
// <engine/Rendering/Renderer.hpp>. Engine's own .cpp files include it
// unprefixed as "Rendering/Renderer.hpp" (see the include in
// Renderer.cpp), which is why this file itself doesn't need an
// "engine/" prefix on anything below.

#include <SDL3/SDL.h>
#include <DirectXMath.h>
#include <cstdint>
#include <vector>

namespace Engine
{
    // Forward declaration only — this header doesn't need BasePart's
    // full definition, just a pointer to it, so we avoid dragging in
    // datamodel/BasePart.h here. Renderer.cpp includes the real thing.
    class BasePart;
}

namespace Engine::Rendering
{
    // A cube mesh vertex: position only. Color now comes from each
    // BasePart's own Color property (pushed as a uniform per draw call)
    // rather than being baked into the mesh, since the same cube mesh
    // is reused for every part.
    struct Vertex
    {
        float x, y, z;
    };

    class Renderer
    {
    public:
        explicit Renderer(SDL_Window* window, SDL_GPUDevice* sharedDevice = nullptr);
        ~Renderer();

        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;

        // Draws every part in `parts` as a box using its Size, its world
        // transform, and its Color.
        //
        // `view` and `projection` are supplied by the caller rather than
        // this class reaching into an Engine::Camera itself — that keeps
        // Renderer from needing to know Camera's API at all. Build them
        // from your camera roughly like:
        //
        //   DirectX::XMMATRIX view = camera->getViewMatrix();
        //   DirectX::XMMATRIX proj = camera->getProjectionMatrix(aspectRatio);
        //
        // (adjust names to match your actual Camera class — I haven't
        // seen it, so I can't confirm those exact method names).

        void DrawFrame(const DirectX::XMMATRIX& view,
            const DirectX::XMMATRIX& projection,
            const std::vector<Engine::BasePart*>& parts);

        SDL_GPUDevice* GetDevice() const { return m_device; }

    private:
        SDL_GPUShader* LoadShader(const char* path,
            SDL_GPUShaderStage stage,
            Uint32 samplerCount,
            Uint32 uniformBufferCount);

        // (Re)creates m_depthTexture if it doesn't exist yet or no longer
        // matches the swapchain's current pixel size — swapchain images
        // resize with the window, so the depth buffer has to track that.
        void EnsureDepthTexture(Uint32 width, Uint32 height);

        SDL_Window* m_window = nullptr;
        SDL_GPUDevice* m_device = nullptr;
        bool m_ownsDevice = false;

        SDL_GPUGraphicsPipeline* m_pipeline = nullptr;
        SDL_GPUBuffer* m_cubeVertexBuffer = nullptr; // shared unit cube, reused per part

        SDL_GPUTexture* m_depthTexture = nullptr;
        Uint32 m_depthWidth = 0;
        Uint32 m_depthHeight = 0;
    };
}