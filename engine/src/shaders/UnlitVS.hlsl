cbuffer MatrixBuffer : register(b0) {
    matrix gViewProjection;
};

struct VSInput {
    float3 position : POSITION;
    float4 color    : COLOR;
};

struct PSInput {
    float4 position : SV_POSITION;
    float4 color    : COLOR;
};

PSInput main(VSInput input) {
    PSInput output;
    // Multiply vertex position by the camera matrix
    output.position = mul(float4(input.position, 1.0f), gViewProjection);
    output.color = input.color;
    return output;
}