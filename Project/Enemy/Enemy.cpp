#include "Enemy.h"

Enemy::Enemy(){
}

Enemy::~Enemy(){
}

void Enemy::Initialize(){
	worldTransform_.Initialize();
	viewProjection_.Initialize();

	texHandle_ = TextureManager::Load("resources/uvChecker.png");
	texHandleBullet_ = TextureManager::Load("resources/black.png");

	model_.reset(Model::CreateObj("cube.obj"));
	model_->SetTexHandle(texHandle_);
	worldTransform_.translate = { 0,2.0f,50.0f };
}

void Enemy::Update(){
	worldTransform_.UpdateMatrix();
	viewProjection_.UpdateMatrix();
	switch (phase_)
	{
	case Phase::Approach:
		MoveApproach();
		break;
	case Phase::Leave:
		MoveLeave();
		break;
	default:
		break;
	}
}

void Enemy::Draw(ViewProjection viewProjection_){
	model_->Draw(worldTransform_, viewProjection_);

}

void Enemy::MoveApproach(){
	//移動
	velocity_ = { 0.0f,0.f,-0.2f };
	worldTransform_.translate = Vector3Add(worldTransform_.translate, velocity_);
	//既定の位置に到達したら離脱
	if (worldTransform_.translate.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::MoveLeave(){
	velocity_ = { -0.2f,0.2f,0.0f };
	worldTransform_.translate = Vector3Add(worldTransform_.translate, velocity_);
}