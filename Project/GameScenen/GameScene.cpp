#include "GameScene.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	
}

// 初期化
void GameScene::Initialize() {
	
	texHandleUV_ = TextureManager::Load("resources/uvChecker.png");
	texHandleCircle_ = TextureManager::Load("resources/enemy.png");
	spriteUV_.reset(Sprite::Create(texHandleUV_));
	spriteCircle_.reset(Sprite::Create(texHandleCircle_));
	
}

// 更新
void GameScene::Update() {
	
	if (input_->PushKey(DIK_S)) {
		sceneNo_ = TITLE;
	}
}

// 描画
void GameScene::Draw() {
	
	spriteUV_->Draw();
	spriteCircle_->Draw();
}

void GameScene::LoadEnemyPopData()
{

}

void GameScene::UpdateEnemyPopCommands()
{
	
}

void GameScene::EnemyInitialize(Vector3)
{
	
}



void GameScene::CheckAllCollisions() {
}