// Vertex shader: transform each cube vertex by the part's model-view-
// projection matrix. No per-vertex color anymore — that comes from the
// part's Color, passed separately to the fragment shader below.

cbuffer MVPBuffer : register(b0, space1) // vertex uniform buffer, slot 0
{
    float4x4 MVP;
};

struct Input
{
    float3 Position : TEXCOORD0;
};

struct Output
{
    float4 Position : SV_Position;
};

Output main(Input input)
{
    Output output;
    output.Position = mul(float4(input.Position, 1.0f), MVP);
    return output;
}
