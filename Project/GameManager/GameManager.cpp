#include "GameManager.h"
#include "GameScene.h"
#include "TitleScene.h"
#include "EndScene.h"

GameManager::GameManager() {
	// 各シーンの配列
	sceneArr_[TITLE] = std::make_unique<TitleScene>();
	sceneArr_[GAME] = std::make_unique<GameScene>();
	sceneArr_[END] = std::make_unique<EndScene>();

	// 初期シーン
	currentSceneNo_ = GAME;

}

GameManager::~GameManager() {}

void GameManager::Run() {


	sceneArr_[currentSceneNo_]->Update();
}

void GameManager::Initialize()
{
	// シーンのチェック
	prevSceneNo_ = currentSceneNo_;
	currentSceneNo_ = sceneArr_[currentSceneNo_]->GetSceneNo();
	// シ－ン変更のチェック
	if (prevSceneNo_ != currentSceneNo_) {
		sceneArr_[currentSceneNo_]->Initialize();

	}

}

void GameManager::Draw()
{
	sceneArr_[currentSceneNo_]->Draw();

}