// A vertex shader runs once per vertex. Its job here is simple: pass the
// position straight through (our triangle is already in the -1..1 clip
// space range the GPU expects) and hand the color along to the fragment
// shader.

struct Input
{
    float3 Position : TEXCOORD0; // matches attribute location = 0
    float4 Color    : TEXCOORD1; // matches attribute location = 1
};

struct Output
{
    float4 Color    : TEXCOORD0;
    float4 Position : SV_Position; // the GPU reads this as clip-space position
};

Output main(Input input)
{
    Output output;
    output.Position = float4(input.Position, 1.0f);
    output.Color = input.Color;
    return output;
}
