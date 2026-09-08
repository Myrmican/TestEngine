cbuffer MatrixBuffer : register(b0) {
    matrix gViewProjectionNoTranslation;
};

TextureCube gSkyboxTexture : register(t0);
SamplerState gSampler : register(s0);

struct VSInput {
    float3 position : POSITION;
};

struct PSInput {
    float4 position : SV_POSITION;
    float3 texCoord : TEXCOORD0;
};

PSInput VSMain(VSInput input) {
    PSInput output;
    
    // Set Z to W so depth value always resolves to 1.0 (far plane)
    float4 pos = mul(float4(input.position, 1.0f), gViewProjectionNoTranslation);
    output.position = pos.xyww;
    output.texCoord = input.position; // Position doubles as 3D direction vector
    
    return output;
}

float4 PSMain(PSInput input) : SV_TARGET {
    return gSkyboxTexture.Sample(gSampler, input.texCoord);
}