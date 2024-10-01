#include "EnemyBullet.h"

void EnemyBullet::Initialize(uint32_t texHandle_, const Vector3& position, const Vector3& velocity) {
	worldTransform_.Initialize();


	model_ = std::make_unique<Object3DPlacer>();
	model_->Initialize();
	model_->SetModel("cube.obj");
	model_->SetTexHandle(texHandle_);
	
	//衝突属性を設定
	SetcollisiionAttribute_(kCollitionAttributeEnemy);
	//衝突対象を自分以外の属性以外に設定
	SetCollisionMask_(~kCollitionAttributeEnemy);

	worldTransform_.translate = position;

	//Z方向に伸びた形状
	worldTransform_.scale.x = 0.5f;
	worldTransform_.scale.y = 0.5f;
	worldTransform_.scale.z = 1.5f;

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
	toPlayer = Subtract(TargetPos_, worldTransform_.translate);
	toPlayer = Normalize(toPlayer);
	velocity_ = Normalize(velocity_);
	velocity_ = Lerp(velocity_, toPlayer, 0.1f);
	velocity_.x *= 0.5f;
	velocity_.y *= 0.5f;
	velocity_.z *= 0.5f;

	// 玉の向き
	worldTransform_.rotate.y = std::atan2(velocity_.x, velocity_.z);
	float VelocityXZ = sqrt((velocity_.x * velocity_.x) + (velocity_.z * velocity_.z));
	worldTransform_.rotate.x = std::atan2(-velocity_.y, VelocityXZ);


	worldTransform_.translate = Add(worldTransform_.translate, velocity_);
}

void EnemyBullet::Draw(const CameraRole& viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_);

#ifdef _RELEASE
	ImGui::Begin("EnemyBullet");
	if (ImGui::TreeNode("worldTransform")) {
		ImGui::DragFloat3("translate", &worldTransform_.translate.x, 0.1f, 100, 100);
		ImGui::DragFloat3("rotate", &worldTransform_.rotate.x, 0.01f, -6.28f, 6.28f);
		ImGui::DragFloat3("scale", &worldTransform_.scale.x, 0.01f, 0, 10);
		ImGui::TreePop();
	}
	ImGui::End();

#endif // RELEASE

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
