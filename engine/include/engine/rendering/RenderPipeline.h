#pragma once

#include <vector>
#include <datamodel/instances/Camera.h>
#include <datamodel/instances/BasePart.h>
#include "rendering/RenderDevice.h"
#include "rendering/Shader.h"
#include "rendering/CubeMesh.h"

namespace Engine {
    struct CameraConstants {
        DirectX::XMMATRIX view;
        DirectX::XMMATRIX projection;
    };

    struct ObjectConstants {
        DirectX::XMMATRIX world;
        DirectX::XMFLOAT3 color;
        float padding = 0.0f;
    };

    class RenderPipeline {
    public:
        bool Init(RenderDevice* device);

        // Temporary scene list - later this becomes "walk the Workspace
        // tree and collect every descendant BasePart" instead of a manual list.
        void AddPart(BasePart* part) { m_parts.push_back(part); }

        void RenderScene(Camera* camera, float aspectRatio);

    private:
        RenderDevice* m_device = nullptr;
        Shader m_shader;
        CubeMesh m_cubeMesh;
        ComPtr<ID3D11Buffer> m_cameraCB;
        ComPtr<ID3D11Buffer> m_objectCB;
        std::vector<BasePart*> m_parts;
    };
}