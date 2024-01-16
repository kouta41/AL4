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
	state = new EnemyStateApproach();
}
;

void Enemy::Update(){
	worldTransform_.UpdateMatrix();
	viewProjection_.UpdateMatrix();
	state->Update(this);
}

void Enemy::Draw(ViewProjection viewProjection_){
	model_->Draw(worldTransform_, viewProjection_);

}

void Enemy::Move(){
	worldTransform_.translate = Vector3Add(worldTransform_.translate, velocity_);

}


void Enemy::SetVelocity(float x, float y, float z) {
	velocity_.x = x;
	velocity_.y = y;
	velocity_.z = z;
}

void Enemy::ChangeState(BaseEnemyState* newState) { state = newState; }


void EnemyStateApproach::Update(Enemy* pEnemy) {
	pEnemy->SetVelocity(0.0f, 0.f, -0.2f);
	//移動
	pEnemy->Move();

	//既定の位置に達したら離脱
	if (pEnemy->GetWorldTransform().z < 0.0f) {
		pEnemy->ChangeState(new EnemyStateLeave());
	}
}


void EnemyStateLeave::Update(Enemy* pEnemy) {
	pEnemy->SetVelocity(-0.2f, 0.2f, 0.0f);
	//移動
	pEnemy->Move();
}