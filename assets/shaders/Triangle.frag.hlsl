// Fragment shader: just output the part's color, pushed in as a
// uniform once per draw call (see Renderer::DrawFrame).

cbuffer ColorBuffer : register(b0, space3) // fragment uniform buffer, slot 0
{
    float4 Color;
};

float4 main() : SV_Target0
{
    return Color;
}
