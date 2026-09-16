#include "rendering/Shader.h"
#include <d3dcompiler.h>
#include <iostream>

using namespace Engine;

bool Shader::Init(ID3D11Device* device, const std::wstring& filePath)
{
    UINT compileFlags = 0;
#ifdef _DEBUG
    compileFlags |= D3DCOMPILE_DEBUG;
#endif

    ComPtr<ID3DBlob> vsBlob, psBlob, errorBlob;

    HRESULT hr = D3DCompileFromFile(filePath.c_str(), nullptr, nullptr,
        "VSMain", "vs_5_0", compileFlags, 0, &vsBlob, &errorBlob);
    if (FAILED(hr))
    {
        if (errorBlob)
        {
            std::cerr << "VS compile error: "
                << static_cast<const char*>(errorBlob->GetBufferPointer()) << std::endl;
        }
        else
        {
            // std::hex is a "sticky" stream manipulator - it stays in effect
            // for the rest of the stream's life until changed back, which is
            // why we reset with std::dec afterward to avoid surprising any
            // later cout usage elsewhere in the app.
            std::cerr << "VS compile failed, HRESULT: 0x" << std::hex << hr << std::dec
                << " (likely file not found - check the path below)" << std::endl;
            std::wcerr << L"Attempted path: " << filePath << std::endl;
        }
        return false;
    }

    hr = D3DCompileFromFile(filePath.c_str(), nullptr, nullptr,
        "PSMain", "ps_5_0", compileFlags, 0, &psBlob, &errorBlob);
    if (FAILED(hr))
    {
        if (errorBlob)
        {
            std::cerr << "PS compile error: "
                << static_cast<const char*>(errorBlob->GetBufferPointer()) << std::endl;
        }
        else
        {
            std::cerr << "PS compile failed, HRESULT: 0x" << std::hex << hr << std::dec << std::endl;
            std::wcerr << L"Attempted path: " << filePath << std::endl;
        }
        return false;
    }

    device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &m_vertexShader);
    device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &m_pixelShader);

    D3D11_INPUT_ELEMENT_DESC layout[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    device->CreateInputLayout(layout, 2, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &m_inputLayout);

    return true;
}