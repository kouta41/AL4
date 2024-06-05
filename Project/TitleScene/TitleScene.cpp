#include "TitleScene.h"

TitleScene::TitleScene() {
}

TitleScene::~TitleScene() {
}

void TitleScene::Initialize(){
	worldTransform.Initialize();
	camera.Initialize();


	texHandle_ = TextureManager::Load("resources/white.png");

}

void TitleScene::Update() {	


	worldTransform.UpdateMatrix();
	camera.UpdateMatrix();
}

void TitleScene::Draw(){
	
}