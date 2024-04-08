#include "Skydome.h"
#include <ModelManager.h>


void Skydome::Initialize(uint32_t texHandle) {
	worldTransform_.Initialize();

	worldTransform_.scale = { 100,100,100 };
	ModelManager::GetInstance()->LoadObjModel("skydome.obj");
	model_ = ModelManager::GetInstance()->CreateObj("skydome.obj");
	model_->SetTexHandle(texHandle);

}

void Skydome::Update() {
	//ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();
}

void Skydome::Draw(CameraRole viewProjection_) {
	//3Dモデル
	model_->Draw(worldTransform_, viewProjection_);

}