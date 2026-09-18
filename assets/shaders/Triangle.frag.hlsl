// A fragment shader runs once per pixel covered by our triangle. It
// receives the color that was interpolated between the 3 vertices'
// colors and just outputs it directly.

struct Input
{
    float4 Color : TEXCOORD0;
};

float4 main(Input input) : SV_Target0
{
    return input.Color;
}
