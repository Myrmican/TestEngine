#include <rendering/Renderer.h>
#include <rendering/util/Shader.h>
#include <datamodel/instances/Camera.h>

namespace Engine
{
    Renderer::~Renderer() { shutdown(); }

    void Renderer::setWireframeMode(bool useWireframe) {
        wireframeMode = useWireframe;
    }

    bool Renderer::init(SDL_Window* window)
    {
        m_window = window;

        Shader::initializeAssetLoader();

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

        SDL_GPUShader* vertexShader = Shader::loadShader(m_device, "RawTriangle.vert");
        SDL_GPUShader* fragmentShader = Shader::loadShader(m_device, "SolidColor.frag");

        SDL_GPUColorTargetDescription colorTargetDesc = {};
        colorTargetDesc.format = SDL_GetGPUSwapchainTextureFormat(m_device, m_window);

        SDL_GPUGraphicsPipelineCreateInfo pipelineInfo = {};
        pipelineInfo.vertex_shader = vertexShader;
        pipelineInfo.fragment_shader = fragmentShader;
        pipelineInfo.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;
        pipelineInfo.target_info.num_color_targets = 1;
        pipelineInfo.target_info.color_target_descriptions = &colorTargetDesc;

        pipelineInfo.rasterizer_state.fill_mode = SDL_GPU_FILLMODE_FILL;
        fillPipeline = SDL_CreateGPUGraphicsPipeline(m_device, &pipelineInfo);
        if (fillPipeline == NULL)
        {
            SDL_Log("Failed to create fill pipeline!");
            SDL_ReleaseGPUShader(m_device, vertexShader);
            SDL_ReleaseGPUShader(m_device, fragmentShader);
            return false;
        }

        pipelineInfo.rasterizer_state.fill_mode = SDL_GPU_FILLMODE_LINE;
        linePipeline = SDL_CreateGPUGraphicsPipeline(m_device, &pipelineInfo);
        if (linePipeline == NULL)
        {
            SDL_Log("Failed to create line pipeline!");
            SDL_ReleaseGPUShader(m_device, vertexShader);
            SDL_ReleaseGPUShader(m_device, fragmentShader);
            return false;
        }

        SDL_ReleaseGPUShader(m_device, vertexShader);
        SDL_ReleaseGPUShader(m_device, fragmentShader);

        return true;
    }

    void Renderer::shutdown()
    {
        if (m_device) {
            SDL_ReleaseGPUGraphicsPipeline(m_device, fillPipeline);
            SDL_ReleaseGPUGraphicsPipeline(m_device, linePipeline);

            SDL_ReleaseWindowFromGPUDevice(m_device, m_window);
            SDL_DestroyGPUDevice(m_device);
            m_device = nullptr;
        }

        m_window = nullptr;

        fillPipeline = nullptr;
        linePipeline = nullptr;
    }
    
    void Renderer::renderFrame(const Engine::Camera& camera)
    {
        if (!m_device || !m_window) return;

        SDL_GPUCommandBuffer* commandBuffer = SDL_AcquireGPUCommandBuffer(m_device);
        if (!commandBuffer) return;

        SDL_GPUTexture* swapchainTexture = nullptr;
        SDL_WaitAndAcquireGPUSwapchainTexture(commandBuffer, m_window, &swapchainTexture, nullptr, nullptr);

        if (swapchainTexture) {
            SDL_GPUColorTargetInfo target = {};
            target.texture = swapchainTexture;
            target.clear_color = SDL_FColor{ 0.10f, 0.25f, 0.60f, 1.0f };
            target.load_op = SDL_GPU_LOADOP_CLEAR;
            target.store_op = SDL_GPU_STOREOP_STORE;

            SDL_GPURenderPass* renderPass = SDL_BeginGPURenderPass(commandBuffer, &target, 1, nullptr);
            SDL_BindGPUGraphicsPipeline(renderPass, wireframeMode ? linePipeline : fillPipeline);
            SDL_DrawGPUPrimitives(renderPass, 3, 1, 0, 0);
            SDL_EndGPURenderPass(renderPass);
        }

        SDL_SubmitGPUCommandBuffer(commandBuffer);
    }
}