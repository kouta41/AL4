#include "EnemyBullet.h"

void EnemyBullet::Initialize(uint32_t texHandle_, const Vector3& position, const Vector3& velocity){
	worldTransform_.Initialize();

	model_.reset(Model::CreateObj("cube.obj"));
	model_->SetTexHandle(texHandle_);

	worldTransform_.translate = position;
	velocity_ = velocity;
}

void EnemyBullet::Update(){
	worldTransform_.UpdateMatrix();

	worldTransform_.translate = Vector3Add(worldTransform_.translate, velocity_);
}

void EnemyBullet::Draw(const ViewProjection& viewProjection_){
	model_->Draw(worldTransform_, viewProjection_);

}
