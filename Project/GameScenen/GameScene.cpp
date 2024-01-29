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
	
	collisionManager_ = new CollisionManager();

}

// 更新
void GameScene::Update() {
	worldTransform_.UpdateMatrix();
	viewProjection_.UpdateMatrix();

	CheckAllCollisions();

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


void GameScene::CheckAllCollisions(){
	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetPlayerBullsts();
	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetEnemyBullsts();
	// 自機のコライダーを登録
	collisionManager_->AddCollider(player_.get());
	// 敵機のコライダーを登録
	collisionManager_->AddCollider(enemy_.get());
	// 自機の弾
	for (PlayerBullet* bullet : playerBullets) {
		collisionManager_->AddCollider(bullet);
	}
	// 敵機の弾
	for (EnemyBullet* bullet : enemyBullets) {
		collisionManager_->AddCollider(bullet);
	}
	collisionManager_->CheckAllCollisions();
	collisionManager_->ClearCollider();
}

