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
	worldTransform_.translate.z -= 0.2f;
}

void Enemy::Draw(ViewProjection viewProjection_){
	model_->Draw(worldTransform_, viewProjection_);

}
