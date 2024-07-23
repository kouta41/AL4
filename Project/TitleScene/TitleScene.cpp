#include "TitleScene.h"
#include <Xinput.h>



TitleScene::TitleScene() {
}

TitleScene::~TitleScene() {
}

void TitleScene::Initialize(){
	worldTransform.Initialize();
	camera.Initialize();

	texHandle_ = TextureManager::Load("resources/TitleDemo.png");
	
	Sprite::StaticInitialize();
	sprite_.reset(Sprite::Create(texHandle_,{0,0}));


	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
}

void TitleScene::Update() {	
	worldTransform.UpdateMatrix();
	camera.UpdateMatrix();
	Sprite::StaticUpdate();

	sprite_->SetPosition(pos);


	if (input_->PushKey(DIK_SPACE)) {
		sceneNo_ = GAME;
	}
}

void TitleScene::Draw(){
	sprite_->Draw();
	

	///デバック場面
	ImGui::Begin("Sprite");
	ImGui::DragFloat2("pos", &pos.x, 0.1f, 100, 100);

	ImGui::End();

	ImGui::Begin("Camera");
	if (ImGui::TreeNode("worldTransform")) {
		ImGui::DragFloat3("translate", &camera.translate.x, 0.1f, 100, 100);
		ImGui::DragFloat("rotateX", &camera.rotate.x, 0.1f, 100, 100);
		ImGui::DragFloat("rotateY", &camera.rotate.y, 0.1f, 100, 100);
		ImGui::DragFloat("rotateZ", &camera.rotate.z, 0.1f, 100, 100);

		ImGui::TreePop();
	}
	ImGui::End();
}

void TitleScene::CheckAllCollisions(){
	

}
