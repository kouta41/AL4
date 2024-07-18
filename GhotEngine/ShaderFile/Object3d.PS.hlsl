#include "Object3d.hlsli"
struct Material
{
    float32_t4 color;
    int32_t enableLighting;
};

struct DirectionalLight
{
    float32_t4 color;
    float32_t3 direction;
    float intensity;
};

struct CameraMatrix
{
    float32_t4x4 view;
    float32_t4x4 projection;
    float32_t3 worldPosition;
};


ConstantBuffer<Material> gMaterial : register(b0);

TextureCube<float32_t4> gEnvironmentTexture : register(t1);

ConstantBuffer<CameraMatrix> gCameraMatrix : register(b2);


struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
	
};

ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    float32_t4 textureColor = gTexture.Sample(gSampler, input.texcoord);
    if (gMaterial.enableLighting != 0)
    {
	    // half lambert
        float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
        float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
		// lambert
	    //float cos = saturate(dot(normalize(input.normal), -gDirectionalLight.direction));
        output.color.rgb = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
        output.color.a = gMaterial.color.a * textureColor.a;
    }
    else
    {
        output.color = gMaterial.color * textureColor;
        
        float32_t3 cameraToPosition = normalize(input.worldPosition - gCameraMatrix.worldPosition);
        float32_t3 reflectedVector = reflect(cameraToPosition, normalize(input.normal));
        float32_t4 environmentColor = gEnvironmentTexture.Sample(gSampler, reflectedVector);
    
        output.color.rgb += environmentColor.rgb;
    }
    
   
    
    return output;

};