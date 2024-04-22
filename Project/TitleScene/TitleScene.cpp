#include "TitleScene.h"

TitleScene::TitleScene() {
}

TitleScene::~TitleScene() {
}

void TitleScene::Initialize(){
	worldTransform.Initialize();
	camera.Initialize();
	matio = std::make_unique<Matio>();
	//model = model_->LoadGLTFFile("./resources/AnimatedCube", "AnimatedCube.gltf");
	animation=matio->LoadAnimationFile("./resources/AnimatedCube", "AnimatedCube.gltf");
	texHandle_ = TextureManager::Load("resources/uvChecker.png");

	matio->SetModelData(model);
	matio->SetAnimation(animation);

	ModelManager::LoadObjModel("cube.obj");
	ModelManager::LoadGLTFModel("AnimatedCube/AnimatedCube.gltf");

	model_ = std::make_unique<Object3DPlacer>();
	model_->Initialize();
	model_->SetModel("AnimatedCube/AnimatedCube.gltf");
//	model_->SetModel("cube.obj");
	model_->SetTexHandle(texHandle_);
}

void TitleScene::Update() {	
	worldTransform.UpdateMatrix();
	camera.UpdateMatrix();
}

void TitleScene::Draw(){
	ImGui::Begin("Player");
	if (ImGui::TreeNode("worldTransform")) {
		ImGui::DragFloat3("translate", &worldTransform.translate.x, 0.1f, 100, 100);
		ImGui::DragFloat3("rotate", &worldTransform.rotate.x, 0.01f, -6.28f, 6.28f);
		ImGui::DragFloat3("scale", &worldTransform.scale.x, 0.01f, 0, 10);
		ImGui::TreePop();
	}
	ImGui::End();
//	matio->Playback(worldTransform, camera);
	model_->Draw(worldTransform, camera);
}