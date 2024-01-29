#include "Skydome.h"


void Skydome::Init(uint32_t texHandle) {
	worldTransform_.Initialize();

	worldTransform_.scale = { 100,100,100 };

	model_.reset(Model::CreateObj("skydome.obj"));
	model_->SetTexHandle(texHandle);


}

void Skydome::Update() {
	//ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();
}

void Skydome::Draw(ViewProjection viewProjection_) {
	//3Dモデル
	model_->Draw(worldTransform_, viewProjection_);

}