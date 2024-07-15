#include "PlayerCore.h"

PlayerCore::PlayerCore(){
}

PlayerCore::~PlayerCore(){
}

void PlayerCore::Initialize(uint32_t texHandle_) {
	worldTransform_.Initialize();

	model_ = std::make_unique<Object3DPlacer>();
	model_->Initialize();
	model_->SetModel("cube.obj");
	model_->SetTexHandle(texHandle_);

	//衝突属性を設定
	SetcollisiionAttribute_(kCollitionAttributePlayer);
	//衝突対象を自分以外の属性以外に設定
	SetCollisionMask_(~kCollitionAttributePlayer);

}

void PlayerCore::Update(Vector3 velocity){
	worldTransform_.UpdateMatrix();
	worldTransform_.translate.x += velocity.x;
	worldTransform_.translate.y += velocity.y;

}

void PlayerCore::Draw(CameraRole viewProjection_){
	model_->Draw(worldTransform_, viewProjection_);

	ImGui::Begin("PlayerCore");
	if (ImGui::TreeNode("worldTransform")) {
		ImGui::DragFloat3("translate", &worldTransform_.translate.x, 0.1f, 100, 100);
		ImGui::DragFloat3("rotate", &worldTransform_.rotate.x, 0.01f, -6.28f, 6.28f);
		ImGui::DragFloat3("scale", &worldTransform_.scale.x, 0.01f, 0, 10);
		ImGui::TreePop();
	}
	ImGui::End();

}

void PlayerCore::OnCollision(){
	isDead_ = true;
}

Vector3 PlayerCore::GetWorldPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld.m[3][0];
	worldPos.y = worldTransform_.matWorld.m[3][1];
	worldPos.z = worldTransform_.matWorld.m[3][2];

	return worldPos;
}

void PlayerCore::SetWorldPosition(Vector3 translate){
	worldTransform_.translate.x = translate.x;
	worldTransform_.translate.y = translate.y;
	worldTransform_.translate.z = translate.z;
}
