#include "EnemyBullet.h"

void EnemyBullet::Initialize(uint32_t texHandle_, const Vector3& position, const Vector3& velocity) {
	worldTransform_.Initialize();

	model_.reset(Model::CreateObj("cube.obj"));
	model_->SetTexHandle(texHandle_);

	worldTransform_.translate = position;

	//Z方向に伸びた形状
	worldTransform_.scale.x = 0.5f;
	worldTransform_.scale.y = 0.5f;
	worldTransform_.scale.z = 3.0f;

	worldTransform_.rotate.y = std::atan2(velocity.x, velocity.z);
	float VelocityXZ = sqrt((velocity.x * velocity.x) + (velocity.z * velocity.z));
	worldTransform_.rotate.x = std::atan2(-velocity.y, VelocityXZ);
	velocity_ = velocity;
}

void EnemyBullet::Update() {
	worldTransform_.UpdateMatrix();

	worldTransform_.translate = Vector3Add(worldTransform_.translate, velocity_);
}

void EnemyBullet::Draw(const ViewProjection& viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_);

}