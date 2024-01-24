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
