#include "WorldTransform.h"

void WorldTransform::Initialize()
{
	// 単位行列を入れておく
	WVP = MakeAffineMatrix(Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f));
	world = MakeIdentityMatrix();
	WorldInverseTranspose = Inverse(MakeIdentityMatrix());


	// 定数バッファ作成
	CreateConstBuffer();
	// mapする
	Map();
	// 転送
	TransferMatrix();

}

void WorldTransform::CreateConstBuffer()
{
	wvpResource = CreateResource::CreateBufferResource(sizeof(TransformationMatrix));
}

void WorldTransform::Map()
{
	wvpResource.Get()->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));
}

void WorldTransform::TransferMatrix()
{
	wvpData->world = world;
	wvpData->WVP = WVP;
	wvpData->WorldInverseTranspose = WorldInverseTranspose;

	/*
	mWvpData->WVP = MakeAffineMatrix(Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f));
	mWvpData->World = MakeIdentity();
	mWvpData->InverseTransposeWorld = Inverse(MakeIdentity());
	*/
}

void WorldTransform::sTransferMatrix(CameraRole camera){
	wvpData->WVP = Multiply(GetWorldMatrix(), Multiply( camera.matView,camera.matProjection));
	wvpData->world = GetWorldMatrix();
	wvpData->WorldInverseTranspose = Inverse(GetWorldMatrix());


}


Matrix4x4 WorldTransform::GetWorldMatrix() const {
	Matrix4x4 result = MakeAffineMatrix(scale, rotate, translate);

	// 親があれば親のワールド行列を掛ける
	if (parent != nullptr) {
		result = Multiply(result, parent->world);
	}
	return result;
}

void WorldTransform::UpdateMatrix()
{

	world = MakeAffineMatrix(scale, rotate, translate);
	// 親があれば親のワールド行列を掛ける
	if (parent) {
		world = Multiply(world, parent->world);
	}

	TransferMatrix();
}