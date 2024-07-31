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
	texHandle_1 = TextureManager::Load("resources/TitleDemoStart.png");

	Sprite::StaticInitialize();
	sprite_.reset(Sprite::Create(texHandle_,{0,0}));
	sprite_1.reset(Sprite::Create(texHandle_1, { 0,0 }));



	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	flag = true;
}

void TitleScene::Update() {	
	worldTransform.UpdateMatrix();
	camera.UpdateMatrix();
	Sprite::StaticUpdate();

	spriteWorldTransform = sprite_->GetWorldTransform();

	if (input_->PressedKey(DIK_SPACE)) {
		flag = false;
	}

	if (flag == false) {
		spriteWorldTransform.rotate.x += 0.02f;

	}

	if (spriteWorldTransform.rotate.x >= 1.65f) {
		sceneNo_ = GAME;

	}
	sprite_->SetWorldTransform(spriteWorldTransform);
	sprite_1->SetWorldTransform(spriteWorldTransform);

}

void TitleScene::Draw(){
	sprite_->Draw();
	sprite_1->Draw();
	///デバック場面
#ifdef RELEASE

	ImGui::Begin("SPACE");
	ImGui::Text("ChangeScene");

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

#endif // RELEASE

}

void TitleScene::CheckAllCollisions(){
	

}
