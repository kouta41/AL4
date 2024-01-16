#include "PlayerBullet.h"

PlayerBullet::PlayerBullet(){
}

PlayerBullet::~PlayerBullet(){
}

void PlayerBullet::Initialize(uint32_t texHandle_, const Vector3& position, const Vector3& velocity) {
	worldTransform_.Initialize();                                                                                 
	viewProjection_.Initialize();

	model_.reset(Model::CreateObj("cube.obj"));
	model_->SetTexHandle(texHandle_);

	worldTransform_.translate = position;
	velocity_ = velocity;
}

void PlayerBullet::Update(){
	worldTransform_.UpdateMatrix();
	viewProjection_.UpdateMatrix();
	worldTransform_.translate = Vector3Add(worldTransform_.translate, velocity_);

	//時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void PlayerBullet::Draw(ViewProjection viewProjection_){
	model_->Draw(worldTransform_, viewProjection_);

}
