#include "Skydome.h"
#include <ModelManager.h>


void Skydome::Initialize(uint32_t texHandle) {
	worldTransform_.Initialize();

	worldTransform_.scale = { 100,100,100 };
	
	model_ = std::make_unique<Object3DPlacer>();
	model_->Initialize();
	model_->SetModel("skydome.obj");
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