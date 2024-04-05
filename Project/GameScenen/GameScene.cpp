#include "GameScene.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	
}

// 初期化
void GameScene::Initialize() {
	
	texHandleUV_ = TextureManager::Load("resources/uvChecker.png");
	texHandleCircle_ = TextureManager::Load("resources/cube.jpg");

	Sprite::StaticInitialize();
	spriteUV_.reset(Sprite::Create(texHandleUV_));
	spriteCircle_.reset(Sprite::Create(texHandleCircle_, { 550.0f,0 }));

}

// 更新
void GameScene::Update() {
	Sprite::StaticUpdate();
	if (input_->PushKey(DIK_S)) {
		sceneNo_ = TITLE;
	}

	ImGui::Begin("SceneChang");
	ImGui::Text("S KEY");
	ImGui::End();
	
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