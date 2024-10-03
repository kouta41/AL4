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
	texHandle_ = TextureManager::Load("resources/gameMane.png");
	texHandle_1 = TextureManager::Load("resources/TitleDemoStart.png");
	SkydometexHandle_ = TextureManager::Load("resources/skydome.jpg");
	blacktexHandle_ = TextureManager::Load("resources/black.png");

	//モデルデータの読み込み
	ModelManager::LoadObjModel("skydome.obj");
	ModelManager::LoadObjModel("cube.obj");


	//天球の生成
	skydome_ = std::make_unique<Skydome>();
	//天球の初期化
	skydome_->Initialize(SkydometexHandle_);

	//スプライトの初期化＆設定
	Sprite::StaticInitialize();
	sprite_.reset(Sprite::Create(texHandle_,{0,0}));
	blackSprite_.reset(Sprite::Create(blacktexHandle_, { 0,0 }));

	//モデルの初期化＆設定
	model_ = std::make_unique<Object3DPlacer>();
	model_->Initialize();
	model_->SetModel("cube.obj");
	model_->SetTexHandle(texHandle_);


	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	flag = true;
}

void TitleScene::Update() {
	//各種更新処理
	worldTransform.UpdateMatrix();
	camera.UpdateMatrix();
	Sprite::StaticUpdate();

	//スプライトの情報をもらう
	spriteWorldTransform = sprite_->GetWorldTransform();


	//天球の更新
	skydome_->Update();


	//演出開始
	if (input_->PressedKey(DIK_SPACE)) {
		flag = false;
	}


	//演出
	if (flag == false) {
		spriteWorldTransform.rotate.x += 0.02f;
	}

	if (spriteWorldTransform.rotate.x >= 1.65f) {
		//次のシーンへ
		//sceneNo_ = SELECT;

	}


	//スプライトに情報を渡す
	sprite_->SetWorldTransform(spriteWorldTransform);
	blackSprite_->SetWorldTransform(spriteWorldTransform);
	blackSprite_->SetColor({1.0f, 1.0f, 1.0f, blackColor_});

}

void TitleScene::Draw(){

	//天球の描画
	skydome_->Draw(camera);

	//モデルの描画
	model_->Draw(worldTransform, camera);

	//スプライトの描画
	//sprite_->Draw();
	blackSprite_->Draw();

	///デバック場面
#ifdef _DEBUG

	ImGui::Begin("WorldTransform");
	if (ImGui::TreeNode("SkydomeWorldTransform")) {
		ImGui::DragFloat3("translate", &skydome_->GetWorldTransform().translate.x, 0.1f, 100, 100);
		ImGui::DragFloat3("rotate", &skydome_->GetWorldTransform().rotate.x, 0.01f, -6.28f, 6.28f);
		ImGui::DragFloat3("scale", &skydome_->GetWorldTransform().scale.x, 0.01f, 0, 10);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("CameraWorldTransform")) {
		ImGui::DragFloat3("translate", &camera.translate.x, 1.0f, 100, 100);
		ImGui::DragFloat3("rotate", &camera.rotate.x, 0.01f, -6.28f, 6.28f);
		ImGui::DragFloat3("scale", &camera.scale.x, 0.01f, 0, 10);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("spriteWorldTransform")) {
		ImGui::DragFloat3("translate", &sprite_->GetWorldTransform().translate.x, 1.0f, 100, 100);
		ImGui::DragFloat3("rotate", &sprite_->GetWorldTransform().rotate.x, 0.01f, -6.28f, 6.28f);
		ImGui::DragFloat3("scale", &sprite_->GetWorldTransform().scale.x, 0.01f, 0, 10);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("modelWorldTransform")) {
		ImGui::DragFloat3("translate", &worldTransform.translate.x, 0.1f, 100, 100);
		ImGui::DragFloat3("rotate", &worldTransform.rotate.x, 0.01f, 100, 100);
		ImGui::DragFloat3("scale", &worldTransform.scale.x, 0.01f, 100, 100);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("blackColor")) {
		ImGui::DragFloat("color", &blackColor_);
		ImGui::TreePop();
	}


	ImGui::End();
	
#endif // _DEBUG



}

void TitleScene::CheckAllCollisions(){
	

}
