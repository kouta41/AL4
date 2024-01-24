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

	//時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	// 玉のホーミング
	toPlayer = Subtract(player_->GetWorldPosition(), worldTransform_.translate);
	toPlayer = Normalize(toPlayer);
	velocity_ = Normalize(velocity_);
	velocity_ = VectorSLerp(velocity_, toPlayer, 0.1f);
	velocity_.x *= 0.5f;
	velocity_.y *= 0.5f;
	velocity_.z *= 0.5f;
	// 玉の向き
	worldTransform_.rotate.y = std::atan2(velocity_.x, velocity_.z);
	float VelocityXZ = sqrt((velocity_.x * velocity_.x) + (velocity_.z * velocity_.z));
	worldTransform_.rotate.x = std::atan2(-velocity_.y, VelocityXZ);


	worldTransform_.translate = Vector3Add(worldTransform_.translate, velocity_);
}

void EnemyBullet::Draw(const ViewProjection& viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_);

}

void EnemyBullet::OnCollision(){
	isDead_ = true;
}

Vector3 EnemyBullet::GetWorldPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld.m[3][0];
	worldPos.y = worldTransform_.matWorld.m[3][1];
	worldPos.z = worldTransform_.matWorld.m[3][2];

	return worldPos;
}
