cbuffer CameraConstants : register(b0)
{
    matrix view;
    matrix projection;
};

cbuffer ObjectConstants : register(b1)
{
    matrix world;
    float3 color;
    float _padding; // cbuffers must be 16-byte aligned; this pads float3 to 16 bytes
};

struct VSInput
{
    float3 position : POSITION;
    float3 normal   : NORMAL;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float3 normal   : NORMAL;
};

PSInput VSMain(VSInput input)
{
    PSInput output;
    float4 worldPos = mul(float4(input.position, 1.0f), world);
    float4 viewPos = mul(worldPos, view);
    output.position = mul(viewPos, projection);

    // Rotate the normal by the world matrix too, so lighting is
    // correct even if the part is rotated. We don't touch translation
    // here (that's why it's a 3x3-style transform via the normal only).
    output.normal = normalize(mul(input.normal, (float3x3)world));
    return output;
}

float4 PSMain(PSInput input) : SV_TARGET
{
    // Simple fixed directional light, coming from upper-front-right.
    // dot(normal, lightDir) is 1.0 when a face directly faces the light,
    // 0 when perpendicular to it - this is what gives the cube visible shape.
    float3 lightDir = normalize(float3(0.5f, 1.0f, -0.5f));
    float brightness = saturate(dot(input.normal, lightDir)) * 0.7f + 0.3f; // 0.3 ambient floor

    return float4(color * brightness, 1.0f);
}