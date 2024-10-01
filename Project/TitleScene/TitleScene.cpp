#include "TitleScene.h"
#include <Xinput.h>



TitleScene::TitleScene() {
}

TitleScene::~TitleScene() {
}

void TitleScene::Initialize(){
	worldTransform.Initialize();
	camera.Initialize();
	camera.translate = { -2,0,-110 };

	ModelManager::LoadObjModel("cube.obj");

	texHandle_ = TextureManager::Load("resources/white.png");
//	texHandle_1 = TextureManager::Load("resources/TitleDemoStart.png");


	//敵キャラの生成
	enemy_ = std::make_unique<Enemy>();
	// 敵キャラの初期化
	enemy_->Initialize();


	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
}

void TitleScene::Update() {	
	worldTransform.UpdateMatrix();
	camera.UpdateMatrix();
	

	//敵の更新
	enemy_->Update();


	if (input_->PressedKey(DIK_SPACE)) {
		sceneNo_ = GAME;
	}


}

void TitleScene::Draw(){
	
	//敵の描画
	enemy_->Draw(camera);


	///デバック場面
#ifdef _DEBUG

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
