#include "PlayerBullet.h"

PlayerBullet::PlayerBullet(){
}

PlayerBullet::~PlayerBullet(){
}

void PlayerBullet::Initialize(uint32_t texHandle_, const Vector3& position, const Vector3& velocity) {
	worldTransform_.Initialize();

	model_ = std::make_unique<Object3DPlacer>();
	model_->Initialize();
	model_->SetModel("cube.obj");
	model_->SetTexHandle(texHandle_);



	worldTransform_.translate = position;
	velocity_ = velocity;
}

void PlayerBullet::Update(){
	worldTransform_.translate = Add(worldTransform_.translate, velocity_);

	//時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
	worldTransform_.UpdateMatrix();
	worldTransform_.scale.x -= 0.1f;
	worldTransform_.scale.y -= 0.1f;
	worldTransform_.scale.z -= 0.1f;

}

void PlayerBullet::Draw(CameraRole viewProjection_){
	model_->Draw(worldTransform_, viewProjection_);

}

void PlayerBullet::OnCollision(){
	isDead_ = true;
}

Vector3 PlayerBullet::GetWorldPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld.m[3][0];
	worldPos.y = worldTransform_.matWorld.m[3][1];
	worldPos.z = worldTransform_.matWorld.m[3][2];

	return worldPos;
}
