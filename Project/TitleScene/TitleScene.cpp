#include "TitleScene.h"

TitleScene::TitleScene() {
}

TitleScene::~TitleScene() {
}

void TitleScene::Initialize(){
	worldTransform.Initialize();
	camera.Initialize();


	texHandle_ = TextureManager::Load("resources/white.png");

	//model = model_->LoadGLTFFile("./resources", "Walk.gltf");
	//animation = matio->LoadAnimationFile("./resources", "Walk.gltf");

	model = model_->LoadGLTFFile("./resources","sneakWalk.gltf");
	animation = matio->LoadAnimationFile("./resources", "sneakWalk.gltf");

	//model = model_->LoadGLTFFile("./resources", "simpleSkin.gltf");
	//animation = matio->LoadAnimationFile("./resources", "simpleSkin.gltf");

	matio->SetTexHandle(texHandle_);
	matio->Initialize(model, animation);
}

void TitleScene::Update() {	
	animationTime += 1.0f / 60.0f;
	//animationTime = fmod(animationTime, 60.0f);
//	animationTime = 2.0f;
	animationTime = std::fmod(animationTime, animation.duration);
	matio->SetanimationTime(animationTime);


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
	ImGui::DragFloat("animationTime", &animationTime);
	ImGui::End();

	matio->Draw(worldTransform,camera);
//	model_->Draw(worldTransform, camera);
}