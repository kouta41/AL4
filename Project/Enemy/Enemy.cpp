 #include "Enemy.h"
#include "Player.h"

Enemy::Enemy(){
}

Enemy::~Enemy(){
	//Enemyの解放
	for (EnemyBullet* EnemyBullet : bullets_) {
		delete EnemyBullet;
	}
}

void Enemy::Initialize(){
	worldTransform_.Initialize();
	viewProjection_.Initialize();

	texHandle_ = TextureManager::Load("resources/enemy.png");
	texHandleBullet_ = TextureManager::Load("resources/black.png");

	model_ = std::make_unique<Object3DPlacer>();
	model_->Initialize();
	model_->SetModel("cube.obj");
	model_->SetTexHandle(texHandle_);
 
	//衝突属性を設定
	SetcollisiionAttribute_(kCollitionAttributeEnemy);
	//衝突対象を自分以外の属性以外に設定
	SetCollisionMask_(~kCollitionAttributeEnemy);

	worldTransform_.translate = { 0,0,0 };
	// 乱数生成器を初期化
	popPosisin = { float(rand() % 10),float(rand() % 10 - 11),0 };
	worldTransform_.translate = popPosisin;
	srand(int(time(0)));

	pop = 0;
	isFire_ = false;
	poprand = 0;
};

void Enemy::Update(){
	worldTransform_.UpdateMatrix();
	viewProjection_.UpdateMatrix();

	//デスフラグの立った弾を削除
	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
		});

	srand((unsigned int)time(NULL)); 	

	///	真下に落ちるタイプ
	if (input_->PressedKey(DIK_A)) {
		// 乱数生成器を初期化
		popPosisin = { float(rand() % 86 - 45),float(rand() % 11 + 27),0 };
		worldTransform_.translate = popPosisin;
		TargetPos_ = popPosisin;
		TargetPos_.y = -24;
		srand((unsigned)time(NULL) * 54321);  // 乱数系列を初期化		UnderFall();
		UnderFall();

	}

	/// 地面に向かって落ちるタイプ
	if (input_->PressedKey(DIK_D)) {
		// 乱数生成器を初期化
		popPosisin = { float(rand() % 86 - 45),float(rand() % 11 + 27),0 };
		worldTransform_.translate = popPosisin;
		srand((unsigned)time(NULL) * 54321);  // 乱数系列を初期化		UnderFall();d
		TargetPos_ = { float(rand() % 86 - 45),-24,0 };
		srand((unsigned)time(NULL) * 54321);  // 乱数系列を初期化		UnderFall();
		TargetFall();
	}


	for (EnemyBullet* bullet_ : bullets_) {
		bullet_->Update();
	}

	

}

///	真下に落ちるタイプ
void Enemy::UnderFall(){
	const float kBulletSpeed = 3.0f;
	Vector3 playerPosition = TargetPos_;
	Vector3 enemyPosition = worldTransform_.translate;
	Vector3 Bulletvelocity = Subtract(playerPosition, enemyPosition);
	Bulletvelocity = Normalize(Bulletvelocity);
	Bulletvelocity.x *= kBulletSpeed;
	Bulletvelocity.y *= kBulletSpeed;
	Bulletvelocity.z *= kBulletSpeed;
	//弾の生成＆初期化
	EnemyBullet* newBullet = new EnemyBullet();

	newBullet->Initialize(texHandleBullet_, worldTransform_.translate, Bulletvelocity);
	newBullet->SetTargetpos(TargetPos_);
	bullets_.push_back(newBullet);
	

}

/// 地面に向かって落ちるタイプ
void Enemy::TargetFall(){
	const float kBulletSpeed = 3.0f;
	Vector3 playerPosition = TargetPos_;
	Vector3 enemyPosition = worldTransform_.translate;
	Vector3 Bulletvelocity = Subtract(playerPosition, enemyPosition);
	Bulletvelocity = Normalize(Bulletvelocity);
	Bulletvelocity.x *= kBulletSpeed;
	Bulletvelocity.y *= kBulletSpeed;
	Bulletvelocity.z *= kBulletSpeed;
	//弾の生成＆初期化
	EnemyBullet* newBullet = new EnemyBullet();

	newBullet->Initialize(texHandleBullet_, worldTransform_.translate, Bulletvelocity);
	newBullet->SetTargetpos(TargetPos_);
	bullets_.push_back(newBullet);
}


void Enemy::Draw(CameraRole viewProjection_){
	// 弾描画
	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection_);
	}

	model_->Draw(worldTransform_, viewProjection_);

//#ifdef Debug

	ImGui::Begin("Enemy");
	if (ImGui::TreeNode("worldTransform")) {
		ImGui::DragFloat3("translate", &worldTransform_.translate.x, 0.1f, 100, 100);
		ImGui::DragFloat3("rotate", &worldTransform_.rotate.x, 0.01f, -6.28f, 6.28f);
		ImGui::DragFloat3("scale", &worldTransform_.scale.x, 0.01f, 0, 10);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("pos")) {
		ImGui::DragFloat3("translate", &kFireInterval.x, 0.1f, 100, 100);
		
		ImGui::TreePop();
	}
	ImGui::End();
//#endif // Debug

}


void Enemy::OnCollision(){
	isDead_ = true;
}

void Enemy::CheckAllCollisions(){

}


void Enemy::SetVelocity(float x, float y, float z) {
	velocity_.x = x;
	velocity_.y = y;
	velocity_.z = z;
}


Vector3 Enemy::GetWorldPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld.m[3][0];
	worldPos.y = worldTransform_.matWorld.m[3][1];
	worldPos.z = worldTransform_.matWorld.m[3][2];

	return worldPos;
}
