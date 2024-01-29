#include "Enemy.h"
#include "Player.h"

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
	worldTransform_.translate = { 25.0f,3.0f,100.0f };

	//衝突属性を設定
	SetcollisiionAttribute_(kCollitionAttributeEnemy);
	//衝突対象を自分以外の属性以外に設定
	SetCollisionMask_(~kCollitionAttributeEnemy);

	state_ = new EnemyStateApproach();

	//最初の状態
	state_ = new EnemyStateApproach();

	ApproachInitialize();
}
;

void Enemy::Update(){
	worldTransform_.UpdateMatrix();
	viewProjection_.UpdateMatrix();
	state_->Update(this);
	for (TimedCall* timedCall_ : timedCalls_) {
		timedCall_->Update();
	}
	timedCalls_.remove_if([](TimedCall* timedCall_) {
		if (timedCall_->IsConpleted()) {
			delete timedCall_;
			return true;
		}
		return false;
		});

	//デスフラグの立った弾を削除
	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
		});


	for (EnemyBullet* bullet_ : bullets_) {
		bullet_->Update();
	}
}

void Enemy::Fire(){
		assert(player_);
		const float kBulletSpeed = 1.0f;
		Vector3 playerPosition = player_->GetWorldPosition();
		Vector3 enemyPosition = this->GetWorldPosition();
		Vector3 Bulletvelocity = Subtract(playerPosition, enemyPosition);
		Bulletvelocity = Normalize(Bulletvelocity);
		Bulletvelocity.x *= kBulletSpeed;
		Bulletvelocity.y *= kBulletSpeed;
		Bulletvelocity.z *= kBulletSpeed;
		//弾の生成＆初期化
		EnemyBullet* newBullet = new EnemyBullet();

		newBullet->Initialize(texHandleBullet_, worldTransform_.translate, Bulletvelocity);
		newBullet->SetPlayer(player_);
		bullets_.push_back(newBullet);
}

void Enemy::Draw(ViewProjection viewProjection_){
	model_->Draw(worldTransform_, viewProjection_);
	for (EnemyBullet* bullet_ : bullets_) {
		bullet_->Draw(viewProjection_);
	}
}

void Enemy::Move(){
	worldTransform_.translate = Vector3Add(worldTransform_.translate, velocity_);
}

void Enemy::OnCollision()
{
}


void Enemy::SetVelocity(float x, float y, float z) {
	velocity_.x = x;
	velocity_.y = y;
	velocity_.z = z;
}

void Enemy::ApproachUpdate()
{
	Fire();
	timedCalls_.push_back(new TimedCall(std::bind(&Enemy::ApproachUpdate, this), 60));
}

void Enemy::ApproachInitialize(){
	timedCalls_.push_back(new TimedCall(std::bind(&Enemy::ApproachUpdate, this), 60));
}

void Enemy::LeaveInitialize(){
	timedCalls_.remove_if([](TimedCall* timedCall_) {
		if (timedCall_ != nullptr) {
			delete timedCall_;
			return true;
		}
		return false;
		});
}

Vector3 Enemy::GetWorldPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld.m[3][0];
	worldPos.y = worldTransform_.matWorld.m[3][1];
	worldPos.z = worldTransform_.matWorld.m[3][2];

	return worldPos;
}

void Enemy::ChangeState(BaseEnemyState* newState) {
	delete state_;
	state_ = newState;
}


void EnemyStateApproach::Update(Enemy* pEnemy) {
	pEnemy->SetVelocity(0.0f, 0.f, -0.2f);
	//既定の位置に達したら離脱
	if (pEnemy->GetWorldTransform().z < 0.0f) {
		pEnemy->LeaveInitialize();
		pEnemy->ChangeState(new EnemyStateLeave());
	}
	//移動
	pEnemy->Move();
}


void EnemyStateLeave::Update(Enemy* pEnemy) {
	/*pEnemy->ApproachInitialize();
	pEnemy->ChangeState(new EnemyStateLeave());*/
	pEnemy->SetVelocity(-0.2f, 0.2f, 0.0f);
	//移動
	pEnemy->Move();
}