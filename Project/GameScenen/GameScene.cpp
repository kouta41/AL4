#include "GameScene.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	
}

// 初期化
void GameScene::Initialize() {
	worldTransform_.Initialize();
	viewProjection_.Initialize();

}

// 更新
void GameScene::Update() {
	
	viewProjection_.UpdateMatrix();
	//worldTransform_.UpdateMatrix();
}

// 描画
void GameScene::Draw() {
	
	
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