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


	if (input_->PressedKey(DIK_SPACE)) {
		//// 乱数生成器を初期化
		//popPosisin = { float(rand() % 45 - 22),float(rand() % 10 - 14),0 };
		//worldTransform_.translate = popPosisin;
		//srand(int(time(0)));
		//LockonFire();
	}

	pop++;
	if(pop >= 30) {
		pop = 0;
	poprand = rand() % 6 + 1;
	isFire_ = true;
	}



	if (isFire_ == true) {
		if (poprand == 1) {
			// 乱数生成器を初期化
			popPosisin = { float(rand() % 45 - 22),float((rand() % 11) + 15),0 };
			worldTransform_.translate = popPosisin;
			//(int(time(0)));
			srand((unsigned)time(NULL) * 54321);  // 乱数系列を初期化

			LockonFire();
		}
		if (poprand == 2) {
			// 乱数生成器を初期化
			popPosisin = { float(rand() % 45 - 22),float(rand() % 10 - 14),0 };
			worldTransform_.translate = popPosisin;
			//srand(int(time(0)));
			srand((unsigned)time(NULL) * 54321);  // 乱数系列を初期化

			LockonFire();
		}
		if (poprand == 3) {
			// 乱数生成器を初期化
			popPosisin = { float((rand() % 11) - 45),float((rand() % 51) - 25),0 };
			worldTransform_.translate = popPosisin;
			//srand(int(time(0)));
			srand((unsigned)time(NULL) * 54321);  // 乱数系列を初期化

			LockonFire();
		}
		if (poprand == 4) {
			// 乱数生成器を初期化
			popPosisin = { float((rand() % 16) + 30),float((rand() % 51) - 25),0 };
			worldTransform_.translate = popPosisin;
			//srand(int(time(0)));
			srand((unsigned)time(NULL) * 54321);  // 乱数系列を初期化

			LockonFire();
		}
		if (poprand == 5) {
			popPosisin = { float((rand() % 11) - 45),float((rand() % 51) - 25),0 };
			worldTransform_.translate = popPosisin;
			TargetPos_ = popPosisin;
			TargetPos_.x = TargetPos_.x * (-1);
			srand((unsigned)time(NULL) * 54321);  // 乱数系列を初期化
			LineFire();
		}
		if (poprand == 6) {
			popPosisin = { float((rand() % 16) + 30),float((rand() % 51) - 25),0 };
			worldTransform_.translate = popPosisin;
			TargetPos_ = popPosisin;
			TargetPos_.x = TargetPos_.x * (-1);
			srand((unsigned)time(NULL) * 54321);  // 乱数系列を初期化

			LineFire();
		}
		isFire_ = false;
	}

	if (input_->PressedKey(DIK_W)) {
		// 乱数生成器を初期化
		popPosisin = { float(rand() % 45 - 22),float((rand() % 11) + 15),0 };
		worldTransform_.translate = popPosisin;
		//(int(time(0)));
		srand((unsigned)time(NULL) * 54321);  // 乱数系列を初期化

		LockonFire();
	}
	if (input_->PressedKey(DIK_S)) {
		// 乱数生成器を初期化
		popPosisin = { float(rand() % 45 - 22),float(rand() % 10 - 14),0 };
		worldTransform_.translate = popPosisin;
		//srand(int(time(0)));
		srand((unsigned)time(NULL) * 54321);  // 乱数系列を初期化

		LockonFire();
	}
	if (input_->PressedKey(DIK_A)) {
		// 乱数生成器を初期化
		popPosisin = { float((rand() % 11) - 45),float((rand() % 51) - 25),0 };
		worldTransform_.translate = popPosisin;
		//srand(int(time(0)));
		srand((unsigned)time(NULL) * 54321);  // 乱数系列を初期化

		LockonFire();
	}
	if (input_->PressedKey(DIK_D)) {
		// 乱数生成器を初期化
		popPosisin = { float((rand() % 16) + 30),float((rand() % 51) - 25),0 };
		worldTransform_.translate = popPosisin;
		//srand(int(time(0)));
		srand((unsigned)time(NULL) * 54321);  // 乱数系列を初期化

		LockonFire();
	}
	if (input_->PressedKey(DIK_Q)) {
		popPosisin = { float((rand() % 11) - 45),float((rand() % 51) - 25),0 };
		worldTransform_.translate = popPosisin;
		TargetPos_ = popPosisin;
		TargetPos_.x = TargetPos_.x * (-1);
		srand((unsigned)time(NULL) * 54321);  // 乱数系列を初期化
		LineFire();
	}
	if (input_->PressedKey(DIK_E)) {
		popPosisin = { float((rand() % 16) + 30),float((rand() % 51) - 25),0 };
		worldTransform_.translate = popPosisin;
		TargetPos_ = popPosisin;
		TargetPos_.x = TargetPos_.x * (-1);
		srand((unsigned)time(NULL) * 54321);  // 乱数系列を初期化

		LineFire();
	}


	for (EnemyBullet* bullet_ : bullets_) {
		bullet_->Update();
	}

	

}

/// <summary>
/// 弾の発射(プレイヤーの核に向かっていく＿追尾はしない)
/// </summary>
void Enemy::LockonFire(){
	const float kBulletSpeed = 1.0f;
	Vector3 playerPosition = coresPos_;
	Vector3 enemyPosition = worldTransform_.translate;
	Vector3 Bulletvelocity = Subtract(playerPosition, enemyPosition);
	Bulletvelocity = Normalize(Bulletvelocity);
	Bulletvelocity.x *= kBulletSpeed;
	Bulletvelocity.y *= kBulletSpeed;
	Bulletvelocity.z *= kBulletSpeed;
	//弾の生成＆初期化
	EnemyBullet* newBullet = new EnemyBullet();

	newBullet->Initialize(texHandleBullet_, worldTransform_.translate, Bulletvelocity);
	newBullet->SetTargetpos(coresPos_);
	bullets_.push_back(newBullet);
}

/// <summary>
/// 弾の発射(ステージの横から直線)
/// </summary>
void Enemy::LineFire(){
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

#ifdef RELEASE

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
#endif // RELEASE

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
