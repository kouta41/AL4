#include "TitleScene.h"
#include <Xinput.h>



TitleScene::TitleScene() {
}

TitleScene::~TitleScene() {
}

void TitleScene::Initialize(){
	//ワールド＆カメラの初期化
	worldTransform.Initialize();
	camera.Initialize();


	//画像の読み込み
	TitletexHandle_ = TextureManager::Load("resources/rainbow.dds");

	//モデルデータの読み込み
	ModelManager::LoadObjModel("cube.obj");


	//画面遷移_右
	model_ = std::make_unique<Object3DPlacer>();
	model_->Initialize();
	model_->SetModel("cube.obj");
	model_->SetTexHandle(TitletexHandle_);
}

void TitleScene::Update() {
	//各種更新処理
	worldTransform.UpdateMatrix();
	camera.UpdateMatrix();

}

void TitleScene::Draw(){
	//画面遷移
	model_->Draw(worldTransform, camera);



	

	///デバック場面
#ifdef _DEBUG

	ImGui::Begin("WorldTransform");
	if (ImGui::TreeNode("worldTransform")) {
		ImGui::DragFloat3("translate", &worldTransform.translate.x, 0.01f, 100, 100);
		ImGui::DragFloat3("rotate", &worldTransform.rotate.x, 0.01f, -6.28f, 6.28f);
		ImGui::DragFloat3("scale", &worldTransform.scale.x, 0.01f, 0, 10);
		ImGui::TreePop();
	}


	ImGui::End();
	
#endif // _DEBUG



}

void TitleScene::CheckAllCollisions(){
	

}
