#include "rendering/Renderer.h"

namespace Engine
{
    Renderer::~Renderer() { shutdown(); }

    bool Renderer::init(SDL_Window* window)
    {
        m_window = window;

        m_device = SDL_CreateGPUDevice(
            SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_MSL,
            true, nullptr);
        if (!m_device) {
            SDL_Log("GPU device creation failed: %s", SDL_GetError());
            return false;
        }

        if (!SDL_ClaimWindowForGPUDevice(m_device, m_window)) {
            SDL_Log("Claiming window failed: %s", SDL_GetError());
            return false;
        }
        return true;
    }

    void Renderer::shutdown()
    {
        if (m_device) {
            SDL_ReleaseWindowFromGPUDevice(m_device, m_window);
            SDL_DestroyGPUDevice(m_device);
            m_device = nullptr;
        }
        m_window = nullptr;
    }

    void Renderer::renderFrame()
    {
        if (!m_device || !m_window) return;

        SDL_GPUCommandBuffer* cmd = SDL_AcquireGPUCommandBuffer(m_device);
        if (!cmd) return;

        SDL_GPUTexture* swapchain = nullptr;
        SDL_WaitAndAcquireGPUSwapchainTexture(cmd, m_window, &swapchain, nullptr, nullptr);

        if (swapchain) {
            SDL_GPUColorTargetInfo target = {};
            target.texture = swapchain;
            target.clear_color = SDL_FColor{ 0.10f, 0.25f, 0.60f, 1.0f };
            target.load_op = SDL_GPU_LOADOP_CLEAR;
            target.store_op = SDL_GPU_STOREOP_STORE;

            SDL_GPURenderPass* pass = SDL_BeginGPURenderPass(cmd, &target, 1, nullptr);
            SDL_EndGPURenderPass(pass);
        }

        SDL_SubmitGPUCommandBuffer(cmd);
    }
}