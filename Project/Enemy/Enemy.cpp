#include "Enemy.h"

Enemy::Enemy(){
}

Enemy::~Enemy(){
	for (EnemyBullet* bullet_ : bullets_) {
		delete bullet_;
	}
}

void Enemy::Initialize(){
	worldTransform_.Initialize();
	viewProjection_.Initialize();

	texHandle_ = TextureManager::Load("resources/uvChecker.png");
	texHandleBullet_ = TextureManager::Load("resources/black.png");

	model_.reset(Model::CreateObj("cube.obj"));
	model_->SetTexHandle(texHandle_);
	worldTransform_.translate = { 25.0f,3.0f,100.0f };
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

	Attack();
	for (EnemyBullet* bullet_ : bullets_) {
		bullet_->Update();
	}
}

void Enemy::Attack(){
	//弾の速度
	const float kBulletSpeed = -1.0f;
	Vector3 velocity(0, 0, kBulletSpeed);
	//弾の生成＆初期化
	EnemyBullet* newBullet = new EnemyBullet();

	newBullet->Initialize(texHandleBullet_, worldTransform_.translate, velocity);

	bullets_.push_back(newBullet);
}

void Enemy::Draw(ViewProjection viewProjection_){
	model_->Draw(worldTransform_, viewProjection_);
	for (EnemyBullet* bullet_ : bullets_) {
		bullet_->Draw(viewProjection_);
	}
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
