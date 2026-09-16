#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <string>

using Microsoft::WRL::ComPtr;

namespace Engine {
    class Shader {
    public:
        bool Init(ID3D11Device* device, const std::wstring& filePath);

        ID3D11VertexShader* getVS() const { return m_vertexShader.Get(); }
        ID3D11PixelShader* getPS() const { return m_pixelShader.Get(); }
        ID3D11InputLayout* getInputLayout() const { return m_inputLayout.Get(); }

    private:
        ComPtr<ID3D11VertexShader> m_vertexShader;
        ComPtr<ID3D11PixelShader> m_pixelShader;
        ComPtr<ID3D11InputLayout> m_inputLayout;
    };
}