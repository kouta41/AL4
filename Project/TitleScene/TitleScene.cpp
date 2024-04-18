#include "TitleScene.h"

TitleScene::TitleScene() {
}

TitleScene::~TitleScene() {
}

void TitleScene::Initialize(){
	worldTransform.Initialize();
	camera.Initialize();
	matio = std::make_unique<Matio>();

	model = model_->LoadGLTFFile("./resources/AnimatedCube", "AnimatedCube.gltf");
	animation=matio->LoadAnimationFile("./resources/AnimatedCube", "AnimatedCube.gltf");
	matio->SetModelData(model);
	matio->SetAnimation(animation);
}

void TitleScene::Update() {	
	worldTransform.UpdateMatrix();
	camera.UpdateMatrix();
}

void TitleScene::Draw(){
	matio->Playback(worldTransform, camera);
}