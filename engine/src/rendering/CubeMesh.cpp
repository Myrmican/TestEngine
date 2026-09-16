#include "rendering/CubeMesh.h"

using namespace Engine;
using namespace DirectX;

bool CubeMesh::Init(ID3D11Device* device)
{
    // 24 vertices, not 8 - each face needs its OWN 4 corners so each
    // can have a distinct, correct normal. Sharing 8 corners between
    // faces would average the normals together and ruin the shading.
    Vertex vertices[] = {
        // +X face
        {{0.5f,-0.5f,-0.5f},{1,0,0}}, {{0.5f, 0.5f,-0.5f},{1,0,0}},
        {{0.5f, 0.5f, 0.5f},{1,0,0}}, {{0.5f,-0.5f, 0.5f},{1,0,0}},
        // -X face
        {{-0.5f,-0.5f, 0.5f},{-1,0,0}}, {{-0.5f, 0.5f, 0.5f},{-1,0,0}},
        {{-0.5f, 0.5f,-0.5f},{-1,0,0}}, {{-0.5f,-0.5f,-0.5f},{-1,0,0}},
        // +Y face
        {{-0.5f, 0.5f,-0.5f},{0,1,0}}, {{-0.5f, 0.5f, 0.5f},{0,1,0}},
        {{0.5f, 0.5f, 0.5f},{0,1,0}}, {{0.5f, 0.5f,-0.5f},{0,1,0}},
        // -Y face
        {{-0.5f,-0.5f, 0.5f},{0,-1,0}}, {{-0.5f,-0.5f,-0.5f},{0,-1,0}},
        {{0.5f,-0.5f,-0.5f},{0,-1,0}}, {{0.5f,-0.5f, 0.5f},{0,-1,0}},
        // +Z face
        {{0.5f,-0.5f, 0.5f},{0,0,1}}, {{0.5f, 0.5f, 0.5f},{0,0,1}},
        {{-0.5f, 0.5f, 0.5f},{0,0,1}}, {{-0.5f,-0.5f, 0.5f},{0,0,1}},
        // -Z face
        {{-0.5f,-0.5f,-0.5f},{0,0,-1}}, {{-0.5f, 0.5f,-0.5f},{0,0,-1}},
        {{0.5f, 0.5f,-0.5f},{0,0,-1}}, {{0.5f,-0.5f,-0.5f},{0,0,-1}},
    };

    // Each face = 2 triangles = 6 indices, referencing the 4 verts above.
    // Winding order (clockwise when viewed from outside) matters for
    // D3D's default backface culling - get this backwards and faces
    // pointing toward the camera get silently culled (invisible).
    UINT indices[] = {
        0,1,2, 0,2,3,       // +X
        4,5,6, 4,6,7,       // -X
        8,9,10, 8,10,11,    // +Y
        12,13,14, 12,14,15, // -Y
        16,17,18, 16,18,19, // +Z
        20,21,22, 20,22,23, // -Z
    };
    m_indexCount = 36;

    D3D11_BUFFER_DESC vbDesc = {};
    vbDesc.ByteWidth = sizeof(vertices);
    vbDesc.Usage = D3D11_USAGE_IMMUTABLE; // never changes after creation - fastest option
    vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    D3D11_SUBRESOURCE_DATA vbData = { vertices };
    if (FAILED(device->CreateBuffer(&vbDesc, &vbData, &m_vertexBuffer)))
        return false;

    D3D11_BUFFER_DESC ibDesc = {};
    ibDesc.ByteWidth = sizeof(indices);
    ibDesc.Usage = D3D11_USAGE_IMMUTABLE;
    ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    D3D11_SUBRESOURCE_DATA ibData = { indices };
    if (FAILED(device->CreateBuffer(&ibDesc, &ibData, &m_indexBuffer)))
        return false;

    return true;
}

void CubeMesh::Draw(ID3D11DeviceContext* context)
{
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
    context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    context->DrawIndexed(m_indexCount, 0, 0);
}