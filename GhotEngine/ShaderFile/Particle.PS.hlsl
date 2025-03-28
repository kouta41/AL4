#include "Particle.hlsli"
struct Material
{
    float32_t4 color;
    int32_t enableLighting;
};


ConstantBuffer<Material> gMaterial : register(b0);
struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
	
};

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    float32_t4 textureColor = gTexture.Sample(gSampler, input.texcoord);

    if (output.color.a == 0.0f)
    {
        discard;
    }
   
    output.color = gMaterial.color * textureColor * input.color;
    return output;

};