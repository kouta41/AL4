#include "Object3d.hlsli"
struct TransformationMatrix
{
    float32_t4x4 WVP;
    float32_t4x4 world;
    float32_t4x4 WorldInverseTranspose;
};

struct CameraMatrix
{
    float32_t4x4 view;
    float32_t4x4 projection;
    float32_t3 worldPosition;
};


ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b0);

ConstantBuffer<CameraMatrix> gCameraMatrix : register(b1);

struct VertexShaderInput
{
    float32_t4 position : POSITION0;
    float32_t2 texcoord : TEXCOORD0;
    float32_t3 normal : NORMAL0;
};


VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    output.position = mul(input.position, gTransformationMatrix.WVP);
    output.texcoord = input.texcoord;
    output.normal = normalize(mul(input.normal, (float32_t3x3) gTransformationMatrix.world));
    output.worldPosition = mul(input.position, gTransformationMatrix.world).xyz;
    return output;
}