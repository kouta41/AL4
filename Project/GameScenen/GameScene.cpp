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
	
}

// 更新
void GameScene::Update() {
	worldTransform_.UpdateMatrix();
	viewProjection_.UpdateMatrix();

	//プレイヤーの更新
	player_->Update();

	
}

// 描画
void GameScene::Draw() {
	//プレイヤーの描画
	player_->Draw(viewProjection_);

}
