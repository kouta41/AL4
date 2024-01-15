#include "PlayerBullet.h"

PlayerBullet::PlayerBullet(){
}

PlayerBullet::~PlayerBullet(){
}

void PlayerBullet::Initialize(uint32_t texHandle_, const Vector3& position) {
	worldTransform_.Initialize();
	viewProjection_.Initialize();

	model_.reset(Model::CreateObj("cube.obj"));
	model_->SetTexHandle(texHandle_);

	worldTransform_.translate = position;
}

void PlayerBullet::Update(){
	worldTransform_.UpdateMatrix();
	viewProjection_.UpdateMatrix();
}

void PlayerBullet::Draw(ViewProjection viewProjection_){
	model_->Draw(worldTransform_, viewProjection_);

}
