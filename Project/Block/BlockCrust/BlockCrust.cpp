#include "BlockCrust.h"

BlockCrust::BlockCrust(){
}

BlockCrust::~BlockCrust(){
}

void BlockCrust::Initialize(uint32_t texHandle_) {
	worldTransform_.Initialize();

	model_ = std::make_unique<Object3DPlacer>();
	model_->Initialize();
	model_->SetModel("cube.obj");
	model_->SetTexHandle(texHandle_);

	// 当たり判定の形状を設定
	SetCollisionPrimitive_(kCollisionAABB);
	//当たり判定の属性
	SetCollisionAttribute_(kAttributeBlock);


}

void BlockCrust::Update(Vector3 velocity){

	worldTransform_.UpdateMatrix();
	worldTransform_.translate.x += velocity.x;
	worldTransform_.translate.y += velocity.y;
}

void BlockCrust::Draw(CameraRole viewProjection_){
	model_->Draw(worldTransform_, viewProjection_);

}

void BlockCrust::OnCollision(){
	isDead_ = true;
}

Vector3 BlockCrust::GetWorldPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld.m[3][0];
	worldPos.y = worldTransform_.matWorld.m[3][1];
	worldPos.z = worldTransform_.matWorld.m[3][2];

	return worldPos;
}

void BlockCrust::SetWorldPosition(Vector3 translate) {
	worldTransform_.translate.x = translate.x;
	worldTransform_.translate.y = translate.y;
	worldTransform_.translate.z = translate.z;
}