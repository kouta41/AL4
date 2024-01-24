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
{	// 判定対象AとBの座標
	Vector3 posA, posB;
	// 自弾リストの取得
	const std::list<PlayerBullet*>& PlayerBullets = player_->GetPlayerBullsts();
	// 敵弾リストの取得
	const std::list<EnemyBullet*>& EnemyBullets = enemy_->GetEnemyBullsts();

#pragma region
	posA = player_->GetWorldPosition();
	for (EnemyBullet* bullet : EnemyBullets) {
		posB = bullet->GetWorldPosition();
		float Length = sqrt(
			(posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) +
			(posB.z - posA.z) * (posB.z - posA.z));

		float Radius = 2.0f;
		if (Length <= Radius) {
			// 自キャラの衝突時コールバック
			player_->OnCollision();
			// 敵の弾の衝突時コールバック
			bullet->OnCollision();
		}
	}
#pragma endregion 自キャラと敵弾の当たり判定

#pragma region
	posA = enemy_->GetWorldPosition();
	for (PlayerBullet* bullet : PlayerBullets) {
		posB = bullet->GetWorldPosition();
		float Length = sqrt(
			(posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) +
			(posB.z - posA.z) * (posB.z - posA.z));

		float Radius = 1.0f;
		if (Length <= Radius) {
			// 自キャラの衝突時コールバック
			player_->OnCollision();
			// 敵の弾の衝突時コールバック
			bullet->OnCollision();
		}
	}
#pragma endregion 自弾と敵キャラの当たり判定

#pragma region

	for (PlayerBullet* Playerbullet : PlayerBullets) {
		posA = Playerbullet->GetWorldPosition();
		for (EnemyBullet* Enemybullet : EnemyBullets) {
			posB = Enemybullet->GetWorldPosition();
			float Length = sqrt(
				(posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) +
				(posB.z - posA.z) * (posB.z - posA.z));

			float Radius = 2.0f;
			if (Length <= Radius) {
				// 自キャラの弾の衝突時コールバック
				Playerbullet->OnCollision();
				// 敵の弾の衝突時コールバック
				Enemybullet->OnCollision();
			}

		}

	}
#pragma endregion 自弾と敵弾の当たり判定
}
