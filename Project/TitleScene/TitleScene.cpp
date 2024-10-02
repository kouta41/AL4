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
	SkydometexHandle_ = TextureManager::Load("resources/skydome.jpg");

	ModelManager::LoadObjModel("skydome.obj");

	//天球の生成
	skydome_ = std::make_unique<Skydome>();
	//天球の初期化
	skydome_->Initialize(SkydometexHandle_);


	Sprite::StaticInitialize();
	sprite_.reset(Sprite::Create(texHandle_,{0,0}));
	sprite_1.reset(Sprite::Create(texHandle_1, { 0,0 }));

	model_ = std::make_unique<Object3DPlacer>();
	model_->Initialize();
	model_->SetModel("cube.obj");
	model_->SetTexHandle(texHandle_);


	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	flag = true;
}

void TitleScene::Update() {	
	worldTransform.UpdateMatrix();
	camera.UpdateMatrix();
	Sprite::StaticUpdate();

	spriteWorldTransform = sprite_->GetWorldTransform();


	//天球の更新
	skydome_->Update();


	if (input_->PressedKey(DIK_SPACE)) {
		flag = false;
	}

	if (flag == false) {
		spriteWorldTransform.rotate.x += 0.02f;

	}

	if (spriteWorldTransform.rotate.x >= 1.65f) {
		sceneNo_ = SELECT;

	}
	sprite_->SetWorldTransform(spriteWorldTransform);
	sprite_1->SetWorldTransform(spriteWorldTransform);

}

void TitleScene::Draw(){
	//sprite_->Draw();
	//sprite_1->Draw();

	//天球の描画
	skydome_->Draw(camera);

	model_->Draw(worldTransform, camera);


	///デバック場面
#ifdef _DEBUG

	ImGui::Begin("Skydome");
	if (ImGui::TreeNode("WorldTransform")) {
		ImGui::DragFloat3("translate", &skydome_->GetWorldTransform().translate.x, 0.1f, 100, 100);
		ImGui::DragFloat3("rotate", &skydome_->GetWorldTransform().rotate.x, 0.01f, -6.28f, 6.28f);
		ImGui::DragFloat3("scale", &skydome_->GetWorldTransform().scale.x, 0.01f, 0, 10);
		ImGui::TreePop();
	}
	ImGui::Text("ChangeScene");

	ImGui::End();
	
	ImGui::Begin("Camera1");

	if (ImGui::TreeNode("CameraWorldTransform")) {
		ImGui::DragFloat3("translate", &camera.translate.x, 0.1f, 100, 100);
		ImGui::DragFloat3("rotate", &camera.rotate.x, 0.01f, -6.28f, 6.28f);
		ImGui::DragFloat3("scale", &camera.scale.x, 0.01f, 0, 10);
		ImGui::TreePop();
	}
	ImGui::End();
#endif // _DEBUG



}

void TitleScene::CheckAllCollisions(){
	

}
