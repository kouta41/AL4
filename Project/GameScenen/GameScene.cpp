#include "GameScene.h"

GameScene::GameScene() {}

GameScene::~GameScene() {

}

// 初期化
void GameScene::Initialize() {
	worldTransform_.Initialize();
	viewProjection_.Initialize();

	//自キャラの生成
	player_ = std::make_unique<Player>();
	player_->Initialize();

	//敵の生成
	enemy_ = std::make_unique<Enemy>();
	enemy_->Initialize();

	enemy_->SetPlayer(player_.get());
	
}

// 更新
void GameScene::Update() {
	worldTransform_.UpdateMatrix();
	viewProjection_.UpdateMatrix();

	CheckAllcollisons();

	//プレイヤーの更新
	player_->Update();

	//敵の更新
	enemy_->Update();

}

// 描画
void GameScene::Draw() {
	//プレイヤーの描画
	player_->Draw(viewProjection_);
	//プレイヤーの描画
	enemy_->Draw(viewProjection_);
}

void GameScene::CheckAllcollisons()
{	
	// 自弾リストの取得
	const std::list<PlayerBullet*>& PlayerBullets = player_->GetPlayerBullsts();
	// 敵弾リストの取得
	const std::list<EnemyBullet*>& EnemyBullets = enemy_->GetEnemyBullsts();

#pragma region 自キャラと敵弾の当たり判定
	for (EnemyBullet* bullet : EnemyBullets) {
		CheckCollisionPair(player_.get(), bullet);
	}
#pragma endregion

#pragma region 自弾と敵キャラの当たり判定
	for (PlayerBullet* bullet : PlayerBullets) {
		CheckCollisionPair(enemy_.get(), bullet);

	}
#pragma endregion 

#pragma region 自弾と敵弾の当たり判定

	for (PlayerBullet* Playerbullet : PlayerBullets) {
		for (EnemyBullet* Enemybullet : EnemyBullets) {
			CheckCollisionPair(Playerbullet, Enemybullet);
		}

	}
#pragma endregion 
}

void GameScene::CheckCollisionPair(Collider* colliderA, Collider* colliderB){
	// 判定対象AとBの座標
	Vector3 posA, posB;
	posA = colliderA->GetWorldPosition();
	posB = colliderB->GetWorldPosition();
	float Length = sqrt(
		(posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) +
		(posB.z - posA.z) * (posB.z - posA.z));
	if (Length <= colliderA->GetRadius() + colliderB->GetRadius()) {
		//コライダーAの衝突時コールバック
		colliderA->OnCollision();
		//コライダーBの衝突時コールバック
		colliderB->OnCollision();
	}
}
