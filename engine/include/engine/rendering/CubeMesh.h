#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

namespace Engine {
    struct Vertex {
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT3 normal;
    };

    class CubeMesh {
    public:
        bool Init(ID3D11Device* device);
        void Draw(ID3D11DeviceContext* context);

    private:
        ComPtr<ID3D11Buffer> m_vertexBuffer;
        ComPtr<ID3D11Buffer> m_indexBuffer;
        UINT m_indexCount = 0;
    };
}