
struct TransformationMatrix
{
    float32_t4x4 matWorld;
    float32_t4x4 world;
    float32_t4x4 WorldInverseTranspose;
};

struct CameraMatrix
{
    float32_t4x4 view;
    float32_t4x4 projection;
    float32_t3 worldPosition;
};

struct Well
{
    float32_t4x4 skeletonSpaceMatrix;
    float32_t4x4 skeletonSpaceInverseTransposeMatrix;
};

struct VertexShaderInput
{
    float32_t4 position : POSITION0;
    float32_t2 texcoord : TEXCOORD0;
    float32_t3 normal : NORMAL0;
    float32_t4 weight : WEIGHT0;
    int32_t4 index : INDEX0;
};

struct VertexShaderOutput
{
    float32_t4 position : SV_POSITION;
    float32_t2 texcoord : TEXCOORD0;
    float32_t3 normal : NORMAL0;
    float32_t3 worldPosition : POSITION0;
    
};

struct Skinned
{
    float32_t4 position;
    float32_t3 normal;
};

ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b0);

ConstantBuffer<CameraMatrix> gCameraMatrix : register(b1);

StructuredBuffer<Well> gMatrixPalette : register(t0);


Skinned Skinning(VertexShaderInput input)
{
    Skinned skinned;
    // 位置の変換
    skinned.position = mul(input.position, gMatrixPalette[input.index.x].skeletonSpaceMatrix) * input.weight.x;
    skinned.position += mul(input.position, gMatrixPalette[input.index.y].skeletonSpaceMatrix) * input.weight.y;
    skinned.position += mul(input.position, gMatrixPalette[input.index.z].skeletonSpaceMatrix) * input.weight.z;
    skinned.position += mul(input.position, gMatrixPalette[input.index.w].skeletonSpaceMatrix) * input.weight.w;
    skinned.position.w = 1.0f;
    
    // 法線の変換
    skinned.normal = mul(input.normal, (float32_t3x3) gMatrixPalette[input.index.x].skeletonSpaceInverseTransposeMatrix) * input.weight.x;
    skinned.normal += mul(input.normal, (float32_t3x3) gMatrixPalette[input.index.y].skeletonSpaceInverseTransposeMatrix) * input.weight.y;
    skinned.normal += mul(input.normal, (float32_t3x3) gMatrixPalette[input.index.z].skeletonSpaceInverseTransposeMatrix) * input.weight.z;
    skinned.normal += mul(input.normal, (float32_t3x3) gMatrixPalette[input.index.w].skeletonSpaceInverseTransposeMatrix) * input.weight.w;
    skinned.normal = normalize(skinned.normal);
    
    return skinned;
}

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    float32_t4x4 wvp = mul(gTransformationMatrix.matWorld, mul(gCameraMatrix.view, gCameraMatrix.projection));
    Skinned skinned = Skinning(input);
    // Skinning結果を使って変換
    output.position = mul(skinned.position, wvp);
    output.texcoord = input.texcoord;
    output.worldPosition = mul(skinned.position, gTransformationMatrix.matWorld).xyz;
    output.normal = normalize(mul(skinned.normal, (float32_t3x3) gTransformationMatrix.WorldInverseTranspose));
    return output;
}