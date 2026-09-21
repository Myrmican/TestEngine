#include "rendering/Renderer.h"

#include <SDL3/SDL_gpu.h>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace Engine::Rendering
{
    static std::vector<Uint8> ReadFile(const char* path)
    {
        std::ifstream file(path, std::ios::binary | std::ios::ate);
        if (!file.is_open())
        {
            throw std::runtime_error(std::string("Could not open file: ") + path);
        }

        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        std::vector<Uint8> buffer(static_cast<size_t>(size));
        if (!file.read(reinterpret_cast<char*>(buffer.data()), size))
        {
            throw std::runtime_error(std::string("Failed to read file: ") + path);
        }
        return buffer;
    }

    // TODO: route this through your engine's actual asset resolution
    // system once it exists, instead of a hardcoded relative path — this
    // will matter especially for the android/ios targets under
    // client/targets, where assets are usually packaged into the app
    // bundle rather than sitting next to the executable.
    static std::string ShaderPath(const char* name)
    {
        return std::string("assets/shaders/compiled/") + name;
    }

    SDL_GPUShader* Renderer::LoadShader(const char* path,
        SDL_GPUShaderStage stage,
        Uint32 samplerCount,
        Uint32 uniformBufferCount)
    {
        std::vector<Uint8> code = ReadFile(path);

        SDL_GPUShaderCreateInfo info{};
        info.code = code.data();
        info.code_size = code.size();
        info.entrypoint = "main";
        info.format = SDL_GPU_SHADERFORMAT_SPIRV; // swap per-platform, see README
        info.stage = stage;
        info.num_samplers = samplerCount;
        info.num_uniform_buffers = uniformBufferCount;
        info.num_storage_buffers = 0;
        info.num_storage_textures = 0;

        SDL_GPUShader* shader = SDL_CreateGPUShader(m_device, &info);
        if (!shader)
        {
            throw std::runtime_error(std::string("Failed to create shader '") +
                path + "': " + SDL_GetError());
        }
        return shader;
    }

    Renderer::Renderer(SDL_Window* window, SDL_GPUDevice* sharedDevice)
        : m_window(window)
    {
        if (sharedDevice)
        {
            //Studio's multi-viewport case: reuse a device another
            //Renderer already created, so all viewports share one GPU
            //connection instead of each opening their own.
            m_device = sharedDevice;
            m_ownsDevice = false;
        }
        else
        {
            m_device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, true, nullptr);
            if (!m_device)
            {
                throw std::runtime_error(
                    std::string("SDL_CreateGPUDevice failed: ") + SDL_GetError());
            }
            m_ownsDevice = true;
        }

        // --- 2. Attach the device to our window (sets up the swapchain) ---
        if (!SDL_ClaimWindowForGPUDevice(m_device, m_window))
        {
            throw std::runtime_error(
                std::string("SDL_ClaimWindowForGPUDevice failed: ") + SDL_GetError());
        }

        SDL_GPUShader* vertexShader = LoadShader(
            ShaderPath("triangle.vert.spv").c_str(), SDL_GPU_SHADERSTAGE_VERTEX, 0, 0);
        SDL_GPUShader* fragmentShader = LoadShader(
            ShaderPath("triangle.frag.spv").c_str(), SDL_GPU_SHADERSTAGE_FRAGMENT, 0, 0);

        SDL_GPUVertexBufferDescription vertexBufferDesc{};
        vertexBufferDesc.slot = 0;
        vertexBufferDesc.pitch = sizeof(Vertex);
        vertexBufferDesc.input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX;
        vertexBufferDesc.instance_step_rate = 0;

        SDL_GPUVertexAttribute attributes[2]{};
        attributes[0].location = 0;
        attributes[0].buffer_slot = 0;
        attributes[0].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3;
        attributes[0].offset = offsetof(Vertex, x);

        attributes[1].location = 1;
        attributes[1].buffer_slot = 0;
        attributes[1].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4;
        attributes[1].offset = offsetof(Vertex, r);

        SDL_GPUColorTargetDescription colorTargetDesc{};
        colorTargetDesc.format = SDL_GetGPUSwapchainTextureFormat(m_device, m_window);

        //Pipeline
        SDL_GPUGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.vertex_shader = vertexShader;
        pipelineInfo.fragment_shader = fragmentShader;
        pipelineInfo.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;

        pipelineInfo.vertex_input_state.vertex_buffer_descriptions = &vertexBufferDesc;
        pipelineInfo.vertex_input_state.num_vertex_buffers = 1;
        pipelineInfo.vertex_input_state.vertex_attributes = attributes;
        pipelineInfo.vertex_input_state.num_vertex_attributes = 2;

        pipelineInfo.target_info.color_target_descriptions = &colorTargetDesc;
        pipelineInfo.target_info.num_color_targets = 1;

        m_pipeline = SDL_CreateGPUGraphicsPipeline(m_device, &pipelineInfo);

        SDL_ReleaseGPUShader(m_device, vertexShader);
        SDL_ReleaseGPUShader(m_device, fragmentShader);

        if (!m_pipeline)
        {
            throw std::runtime_error(
                std::string("SDL_CreateGPUGraphicsPipeline failed: ") + SDL_GetError());
        }

        Vertex triangle[3] = {
            {   0.0f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f },
            {   0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f },
            {  -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f },
        };

        SDL_GPUBufferCreateInfo bufferInfo{};
        bufferInfo.usage = SDL_GPU_BUFFERUSAGE_VERTEX;
        bufferInfo.size = sizeof(triangle);
        m_vertexBuffer = SDL_CreateGPUBuffer(m_device, &bufferInfo);

        SDL_GPUTransferBufferCreateInfo transferInfo{};
        transferInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
        transferInfo.size = sizeof(triangle);
        SDL_GPUTransferBuffer* transferBuffer =
            SDL_CreateGPUTransferBuffer(m_device, &transferInfo);

        void* mapped = SDL_MapGPUTransferBuffer(m_device, transferBuffer, false);
        SDL_memcpy(mapped, triangle, sizeof(triangle));
        SDL_UnmapGPUTransferBuffer(m_device, transferBuffer);

        SDL_GPUCommandBuffer* uploadCmdBuf = SDL_AcquireGPUCommandBuffer(m_device);
        SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(uploadCmdBuf);

        SDL_GPUTransferBufferLocation source{};
        source.transfer_buffer = transferBuffer;
        source.offset = 0;

        SDL_GPUBufferRegion destination{};
        destination.buffer = m_vertexBuffer;
        destination.offset = 0;
        destination.size = sizeof(triangle);

        SDL_UploadToGPUBuffer(copyPass, &source, &destination, false);
        SDL_EndGPUCopyPass(copyPass);
        SDL_SubmitGPUCommandBuffer(uploadCmdBuf);

        SDL_ReleaseGPUTransferBuffer(m_device, transferBuffer);
    }

    Renderer::~Renderer()
    {
        if (m_vertexBuffer) SDL_ReleaseGPUBuffer(m_device, m_vertexBuffer);
        if (m_pipeline) SDL_ReleaseGPUGraphicsPipeline(m_device, m_pipeline);
        if (m_device && m_window) SDL_ReleaseWindowFromGPUDevice(m_device, m_window);

        // Only destroy the device if we created it ourselves — if it was
        // shared in from another Renderer (studio's multi-viewport case),
        // whoever created it owns destroying it.
        if (m_ownsDevice && m_device) SDL_DestroyGPUDevice(m_device);
    }

    void Renderer::DrawFrame()
    {
        SDL_GPUCommandBuffer* cmdBuf = SDL_AcquireGPUCommandBuffer(m_device);
        if (!cmdBuf)
        {
            SDL_Log("SDL_AcquireGPUCommandBuffer failed: %s", SDL_GetError());
            return;
        }

        SDL_GPUTexture* swapchainTexture = nullptr;
        if (!SDL_WaitAndAcquireGPUSwapchainTexture(cmdBuf, m_window, &swapchainTexture,
            nullptr, nullptr))
        {
            SDL_Log("SDL_WaitAndAcquireGPUSwapchainTexture failed: %s", SDL_GetError());
            SDL_SubmitGPUCommandBuffer(cmdBuf);
            return;
        }

        if (!swapchainTexture)
        {
            SDL_SubmitGPUCommandBuffer(cmdBuf);
            return;
        }

        SDL_GPUColorTargetInfo colorTargetInfo{};
        colorTargetInfo.texture = swapchainTexture;
        colorTargetInfo.clear_color = SDL_FColor{ 0.1f, 0.1f, 0.15f, 1.0f };
        colorTargetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
        colorTargetInfo.store_op = SDL_GPU_STOREOP_STORE;

        SDL_GPURenderPass* renderPass =
            SDL_BeginGPURenderPass(cmdBuf, &colorTargetInfo, 1, nullptr);

        SDL_BindGPUGraphicsPipeline(renderPass, m_pipeline);

        SDL_GPUBufferBinding vertexBinding{};
        vertexBinding.buffer = m_vertexBuffer;
        vertexBinding.offset = 0;
        SDL_BindGPUVertexBuffers(renderPass, 0, &vertexBinding, 1);

        SDL_DrawGPUPrimitives(renderPass, 3, 1, 0, 0);

        SDL_EndGPURenderPass(renderPass);
        SDL_SubmitGPUCommandBuffer(cmdBuf);
    }
}