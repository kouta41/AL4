#include "CameraRole.h"



void CameraRole::Initialize() {
	CreateConstBuffer();
	Map();
	UpdateMatrix();
	TransferMatrix();
}

void CameraRole::CreateConstBuffer() {
	constBuff_ = CreateResource::CreateBufferResource(sizeof(ConstBufferDataViewProjection));
}

void CameraRole::Map() {
	constBuff_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&constMap));
}

void CameraRole::UpdateMatrix() {
	UpdateViewMatrix();
	UpdateProjectionMatrix();
	TransferMatrix();
}

void CameraRole::TransferMatrix() {
	constMap->view = matView;
	constMap->projection = matProjection;
	constMap->cameraPos = { matView.m[3][0], matView.m[3][1], matView.m[3][2] };
}

void CameraRole::UpdateViewMatrix() {
	Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, rotate, translate);
	matView = Inverse(cameraMatrix);
	worldPos_ = { matView.m[3][0], matView.m[3][1], matView.m[3][2] };

}

void CameraRole::UpdateProjectionMatrix() {
	matProjection = MakePerspectiveFovMatrix(fovAngleY, aspectRatio, nearZ, farZ);
}