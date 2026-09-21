#include "rendering/Renderer.h"
#include <datamodel/instances/BasePart.h>
#include <core/math/Color3.h>
#include <core/math/Vector3.h>

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

    static std::string ShaderPath(const char* name)
    {
        return std::string("assets/shaders/compiled/") + name;
    }

    // --- A unit cube: -0.5..0.5 on every axis, so a BasePart's Size can
    // be applied as a direct scale to get a box of that exact size. ---
    // Position only (no per-vertex color anymore — color now comes from
    // each part via a uniform, since one cube mesh is shared and redrawn
    // once per part with different transform + color each time).
    // 6 faces x 2 triangles x 3 vertices, no index buffer — simplest to
    // read even though it repeats shared corners.
    static const Vertex kCubeVertices[36] = {
        // -Z face
        {-0.5f,-0.5f,-0.5f}, { 0.5f,-0.5f,-0.5f}, { 0.5f, 0.5f,-0.5f},
        {-0.5f,-0.5f,-0.5f}, { 0.5f, 0.5f,-0.5f}, {-0.5f, 0.5f,-0.5f},
        // +Z face
        {-0.5f,-0.5f, 0.5f}, { 0.5f, 0.5f, 0.5f}, { 0.5f,-0.5f, 0.5f},
        {-0.5f,-0.5f, 0.5f}, {-0.5f, 0.5f, 0.5f}, { 0.5f, 0.5f, 0.5f},
        // -X face
        {-0.5f,-0.5f, 0.5f}, {-0.5f,-0.5f,-0.5f}, {-0.5f, 0.5f,-0.5f},
        {-0.5f,-0.5f, 0.5f}, {-0.5f, 0.5f,-0.5f}, {-0.5f, 0.5f, 0.5f},
        // +X face
        { 0.5f,-0.5f,-0.5f}, { 0.5f,-0.5f, 0.5f}, { 0.5f, 0.5f, 0.5f},
        { 0.5f,-0.5f,-0.5f}, { 0.5f, 0.5f, 0.5f}, { 0.5f, 0.5f,-0.5f},
        // +Y face
        {-0.5f, 0.5f,-0.5f}, { 0.5f, 0.5f,-0.5f}, { 0.5f, 0.5f, 0.5f},
        {-0.5f, 0.5f,-0.5f}, { 0.5f, 0.5f, 0.5f}, {-0.5f, 0.5f, 0.5f},
        // -Y face
        {-0.5f,-0.5f,-0.5f}, { 0.5f,-0.5f, 0.5f}, { 0.5f,-0.5f,-0.5f},
        {-0.5f,-0.5f,-0.5f}, {-0.5f,-0.5f, 0.5f}, { 0.5f,-0.5f, 0.5f},
    };

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
        info.format = SDL_GPU_SHADERFORMAT_SPIRV;
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

        if (!SDL_ClaimWindowForGPUDevice(m_device, m_window))
        {
            throw std::runtime_error(
                std::string("SDL_ClaimWindowForGPUDevice failed: ") + SDL_GetError());
        }

        // Vertex shader now takes 1 uniform buffer (the MVP matrix).
        // Fragment shader now takes 1 uniform buffer too (the color).
        SDL_GPUShader* vertexShader = LoadShader(
            ShaderPath("triangle.vert.spv").c_str(), SDL_GPU_SHADERSTAGE_VERTEX, 0, 1);
        SDL_GPUShader* fragmentShader = LoadShader(
            ShaderPath("triangle.frag.spv").c_str(), SDL_GPU_SHADERSTAGE_FRAGMENT, 0, 1);

        // Vertex layout: just a float3 position now.
        SDL_GPUVertexBufferDescription vertexBufferDesc{};
        vertexBufferDesc.slot = 0;
        vertexBufferDesc.pitch = sizeof(Vertex);
        vertexBufferDesc.input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX;
        vertexBufferDesc.instance_step_rate = 0;

        SDL_GPUVertexAttribute attribute{};
        attribute.location = 0;
        attribute.buffer_slot = 0;
        attribute.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3;
        attribute.offset = 0;

        SDL_GPUColorTargetDescription colorTargetDesc{};
        colorTargetDesc.format = SDL_GetGPUSwapchainTextureFormat(m_device, m_window);

        // --- Depth/stencil state ---
        // A cube has faces that occlude each other; without depth
        // testing, triangles just draw in whatever order we issue them,
        // not front-to-back order, so far faces can incorrectly paint
        // over near ones. D32_FLOAT is a broadly-supported depth-only
        // format across SDL_GPU's backends; if you add shadow mapping or
        // similar later and need to sample this texture, you'll want to
        // revisit the format choice.
        SDL_GPUDepthStencilState depthStencilState{};
        depthStencilState.enable_depth_test = true;
        depthStencilState.enable_depth_write = true;
        depthStencilState.compare_op = SDL_GPU_COMPAREOP_LESS;

        SDL_GPUGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.vertex_shader = vertexShader;
        pipelineInfo.fragment_shader = fragmentShader;
        pipelineInfo.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;

        pipelineInfo.vertex_input_state.vertex_buffer_descriptions = &vertexBufferDesc;
        pipelineInfo.vertex_input_state.num_vertex_buffers = 1;
        pipelineInfo.vertex_input_state.vertex_attributes = &attribute;
        pipelineInfo.vertex_input_state.num_vertex_attributes = 1;

        // Deliberately NOT culling back faces yet. Getting winding wrong
        // relative to your camera's handedness convention would make
        // the whole cube disappear with no error message, which is a
        // much worse debugging experience than a few wasted triangles.
        // Enable SDL_GPU_CULLMODE_BACK later once you've confirmed the
        // cube looks right and know your coordinate convention.
        pipelineInfo.rasterizer_state.cull_mode = SDL_GPU_CULLMODE_NONE;

        pipelineInfo.depth_stencil_state = depthStencilState;
        pipelineInfo.target_info.color_target_descriptions = &colorTargetDesc;
        pipelineInfo.target_info.num_color_targets = 1;
        pipelineInfo.target_info.depth_stencil_format = SDL_GPU_TEXTUREFORMAT_D32_FLOAT;
        pipelineInfo.target_info.has_depth_stencil_target = true;

        m_pipeline = SDL_CreateGPUGraphicsPipeline(m_device, &pipelineInfo);

        SDL_ReleaseGPUShader(m_device, vertexShader);
        SDL_ReleaseGPUShader(m_device, fragmentShader);

        if (!m_pipeline)
        {
            throw std::runtime_error(
                std::string("SDL_CreateGPUGraphicsPipeline failed: ") + SDL_GetError());
        }

        // --- Upload the shared unit cube (once, reused for every part) ---
        SDL_GPUBufferCreateInfo bufferInfo{};
        bufferInfo.usage = SDL_GPU_BUFFERUSAGE_VERTEX;
        bufferInfo.size = sizeof(kCubeVertices);
        m_cubeVertexBuffer = SDL_CreateGPUBuffer(m_device, &bufferInfo);

        SDL_GPUTransferBufferCreateInfo transferInfo{};
        transferInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
        transferInfo.size = sizeof(kCubeVertices);
        SDL_GPUTransferBuffer* transferBuffer =
            SDL_CreateGPUTransferBuffer(m_device, &transferInfo);

        void* mapped = SDL_MapGPUTransferBuffer(m_device, transferBuffer, false);
        SDL_memcpy(mapped, kCubeVertices, sizeof(kCubeVertices));
        SDL_UnmapGPUTransferBuffer(m_device, transferBuffer);

        SDL_GPUCommandBuffer* uploadCmdBuf = SDL_AcquireGPUCommandBuffer(m_device);
        SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(uploadCmdBuf);

        SDL_GPUTransferBufferLocation source{};
        source.transfer_buffer = transferBuffer;
        source.offset = 0;

        SDL_GPUBufferRegion destination{};
        destination.buffer = m_cubeVertexBuffer;
        destination.offset = 0;
        destination.size = sizeof(kCubeVertices);

        SDL_UploadToGPUBuffer(copyPass, &source, &destination, false);
        SDL_EndGPUCopyPass(copyPass);
        SDL_SubmitGPUCommandBuffer(uploadCmdBuf);

        SDL_ReleaseGPUTransferBuffer(m_device, transferBuffer);
    }

    Renderer::~Renderer()
    {
        if (m_depthTexture) SDL_ReleaseGPUTexture(m_device, m_depthTexture);
        if (m_cubeVertexBuffer) SDL_ReleaseGPUBuffer(m_device, m_cubeVertexBuffer);
        if (m_pipeline) SDL_ReleaseGPUGraphicsPipeline(m_device, m_pipeline);
        if (m_device && m_window) SDL_ReleaseWindowFromGPUDevice(m_device, m_window);
        if (m_ownsDevice && m_device) SDL_DestroyGPUDevice(m_device);
    }

    void Renderer::EnsureDepthTexture(Uint32 width, Uint32 height)
    {
        if (m_depthTexture && m_depthWidth == width && m_depthHeight == height)
        {
            return; // already the right size
        }

        if (m_depthTexture)
        {
            SDL_ReleaseGPUTexture(m_device, m_depthTexture);
            m_depthTexture = nullptr;
        }

        SDL_GPUTextureCreateInfo textureInfo{};
        textureInfo.type = SDL_GPU_TEXTURETYPE_2D;
        textureInfo.format = SDL_GPU_TEXTUREFORMAT_D32_FLOAT;
        textureInfo.usage = SDL_GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET;
        textureInfo.width = width;
        textureInfo.height = height;
        textureInfo.layer_count_or_depth = 1;
        textureInfo.num_levels = 1;

        m_depthTexture = SDL_CreateGPUTexture(m_device, &textureInfo);
        if (!m_depthTexture)
        {
            SDL_Log("SDL_CreateGPUTexture (depth) failed: %s", SDL_GetError());
        }
        m_depthWidth = width;
        m_depthHeight = height;
    }

    void Renderer::DrawFrame(const DirectX::XMMATRIX& view,
        const DirectX::XMMATRIX& projection,
        const std::vector<Engine::BasePart*>& parts)
    {
        SDL_GPUCommandBuffer* cmdBuf = SDL_AcquireGPUCommandBuffer(m_device);
        if (!cmdBuf)
        {
            SDL_Log("SDL_AcquireGPUCommandBuffer failed: %s", SDL_GetError());
            return;
        }

        SDL_GPUTexture* swapchainTexture = nullptr;
        Uint32 swapchainWidth = 0, swapchainHeight = 0;
        if (!SDL_WaitAndAcquireGPUSwapchainTexture(cmdBuf, m_window, &swapchainTexture,
            &swapchainWidth, &swapchainHeight))
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

        EnsureDepthTexture(swapchainWidth, swapchainHeight);

        SDL_GPUColorTargetInfo colorTargetInfo{};
        colorTargetInfo.texture = swapchainTexture;
        colorTargetInfo.clear_color = SDL_FColor{ 1.0f, 0.5f, 0.15f, 1.0f };
        colorTargetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
        colorTargetInfo.store_op = SDL_GPU_STOREOP_STORE;

        SDL_GPUDepthStencilTargetInfo depthTargetInfo{};
        depthTargetInfo.texture = m_depthTexture;
        depthTargetInfo.clear_depth = 1.0f;
        depthTargetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
        depthTargetInfo.store_op = SDL_GPU_STOREOP_DONT_CARE;
        depthTargetInfo.stencil_load_op = SDL_GPU_LOADOP_DONT_CARE;
        depthTargetInfo.stencil_store_op = SDL_GPU_STOREOP_DONT_CARE;

        SDL_GPURenderPass* renderPass =
            SDL_BeginGPURenderPass(cmdBuf, &colorTargetInfo, 1, &depthTargetInfo);
        if (!renderPass)
        {
            SDL_Log("SDL_BeginGPURenderPass failed: %s", SDL_GetError());
            SDL_SubmitGPUCommandBuffer(cmdBuf);
            return;
        }

        SDL_BindGPUGraphicsPipeline(renderPass, m_pipeline);

        SDL_GPUBufferBinding vertexBinding{};
        vertexBinding.buffer = m_cubeVertexBuffer;
        vertexBinding.offset = 0;
        SDL_BindGPUVertexBuffers(renderPass, 0, &vertexBinding, 1);

        for (Engine::BasePart* part : parts)
        {
            if (!part) continue;

            // Model transform: your engine's existing getWorldMatrix()
            // is assumed to already fold in position, rotation, and
            // Size scaling. If it turns out to only be CFrame (position
            // + rotation, no scale), you'd instead do:
            //   DirectX::XMMATRIX scale = DirectX::XMMatrixScalingFromVector(
            //       DirectX::XMVectorSet(size.x, size.y, size.z, 0.0f));
            //   DirectX::XMMATRIX model = scale * part->getCFrame();
            DirectX::XMMATRIX model = part->getWorldMatrix();

            DirectX::XMMATRIX mvp = model * view * projection;

            // DirectXMath matrices are row-major in CPU memory; HLSL
            // constant buffers default to column-major layout. This
            // transpose is what makes `mul(vector, MVP)` in the shader
            // do the same thing you'd expect from the untransposed
            // math on the CPU side — standard D3D idiom.
            DirectX::XMFLOAT4X4 mvpForGpu;
            DirectX::XMStoreFloat4x4(&mvpForGpu, DirectX::XMMatrixTranspose(mvp));

            SDL_PushGPUVertexUniformData(cmdBuf, 0, &mvpForGpu, sizeof(mvpForGpu));

            // TODO: adjust field names if your Color3 doesn't expose
            // r/g/b directly (e.g. if it's getR()/getG()/getB() instead).
            Color3 color = part->getColor();
            float colorForGpu[4] = { color.m_red, color.m_green, color.m_blue, 1.0f };
            SDL_PushGPUFragmentUniformData(cmdBuf, 0, colorForGpu, sizeof(colorForGpu));

            SDL_DrawGPUPrimitives(renderPass, 36, 1, 0, 0);
        }

        SDL_EndGPURenderPass(renderPass);
        SDL_SubmitGPUCommandBuffer(cmdBuf);
    }
}