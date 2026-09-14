cbuffer PerObjectBuffer : register(b0) {
    matrix gWorldViewProjection;
    float4 gPartColor;
};

struct VSInput {
    float3 position : POSITION;
};

struct PSInput {
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

PSInput main(VSInput input) {
    PSInput output;
    output.position = mul(gWorldViewProjection, float4(input.position, 1.0f));
    output.color = gPartColor;
    return output;
}