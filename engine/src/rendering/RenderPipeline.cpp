#include "rendering/RenderPipeline.h"
#include "core/FileSystem.h"
#include <iostream>

using namespace Engine;
using namespace DirectX;

bool RenderPipeline::Init(RenderDevice* device)
{
    m_device = device;
    auto* d3d = device->getDevice();

    //std::wstring shaderPath = getExecutableDirectory() + L"shaders/Basic.hlsl";

    if (!m_shader.Init(d3d, L"engine/rendering/shaders/Basic.hlsl"))
        std::cout << "RenderPipeline::Init - Shader::Init failed";
        return false;
    if (!m_cubeMesh.Init(d3d))
        std::cout << "RenderPipeline::Init - CubeMesh::Init failed";
        return false;

    auto makeConstantBuffer = [&](UINT byteWidth, ComPtr<ID3D11Buffer>& out) {
        D3D11_BUFFER_DESC desc = {};
        desc.ByteWidth = byteWidth;
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        return SUCCEEDED(d3d->CreateBuffer(&desc, nullptr, &out));
        };

    if (!makeConstantBuffer(sizeof(CameraConstants), m_cameraCB)) std::cout << "RenderPipeline::Init - camera constant buffer creation failed"; return false;
    if (!makeConstantBuffer(sizeof(ObjectConstants), m_objectCB)) std::cout << "RenderPipeline::Init - object constant buffer creation failed"; return false;

    return true;
}

void RenderPipeline::RenderScene(Camera* camera, float aspectRatio)
{
    auto* context = m_device->getContext();

    // --- 1. Camera constants: bound ONCE per frame, not per object ---
    if (camera)
    {
        CameraConstants cc;
        cc.view = XMMatrixTranspose(camera->getViewMatrix());
        cc.projection = XMMatrixTranspose(camera->getProjectionMatrix(aspectRatio));

        D3D11_MAPPED_SUBRESOURCE mapped;
        if (SUCCEEDED(context->Map(m_cameraCB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped)))
        {
            memcpy(mapped.pData, &cc, sizeof(cc));
            context->Unmap(m_cameraCB.Get(), 0);
        }
        context->VSSetConstantBuffers(0, 1, m_cameraCB.GetAddressOf()); // b0
    }

    // --- 2. Bind shaders + input layout (same for every part right now) ---
    context->IASetInputLayout(m_shader.getInputLayout());
    context->VSSetShader(m_shader.getVS(), nullptr, 0);
    context->PSSetShader(m_shader.getPS(), nullptr, 0);

    // --- 3. Loop every part: update per-object constants, then draw ---
    for (BasePart* part : m_parts) {
        Vector3 size = part->getSize();
        Color3 color = part->getColor();

        // Scale (by size) then place (by CFrame) - scale must happen
        // BEFORE the CFrame's rotation/translation, which is why it's
        // the rightmost multiplication (matrices apply right-to-left here).
        XMMATRIX scale = XMMatrixScaling(size.x, size.y, size.z);
        XMMATRIX world = scale * part->getCFrame();

        ObjectConstants oc;
        oc.world = XMMatrixTranspose(world);
        oc.color = XMFLOAT3(color.m_red, color.m_green, color.m_blue);

        D3D11_MAPPED_SUBRESOURCE mapped;
        if (SUCCEEDED(context->Map(m_objectCB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped)))
        {
            memcpy(mapped.pData, &oc, sizeof(oc));
            context->Unmap(m_objectCB.Get(), 0);
        }
        context->VSSetConstantBuffers(1, 1, m_objectCB.GetAddressOf()); // b1

        m_cubeMesh.Draw(context);
    }
}