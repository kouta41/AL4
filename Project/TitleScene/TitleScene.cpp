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
	TitletexHandle_ = TextureManager::Load("resources/7g9a5.png");
	objecttexHandle_R = TextureManager::Load("resources/81c9l.png");
	objecttexHandle_L = TextureManager::Load("resources/w14s8.png");
	StarttexHandle_ = TextureManager::Load("resources/7nu95.png");

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
	sprite_.reset(Sprite::Create(TitletexHandle_,{0,0}));
	blackSprite_.reset(Sprite::Create(blacktexHandle_, { 0,0 }));

	//モデルの初期化＆設定(タイトル)
	Titlemodel_ = std::make_unique<Object3DPlacer>();
	Titlemodel_->Initialize();
	Titlemodel_->SetModel("cube.obj");
	Titlemodel_->SetTexHandle(TitletexHandle_);
	TitleworldTransform_.Initialize();
	TitleworldTransform_.translate = { 0.0f,0.63f,-50.75f };
	TitleworldTransform_.scale = { 1.5f,1.0f,1.0f };
	//モデルの初期化＆設定(オブジェクト右)
	objectemodel_R = std::make_unique<Object3DPlacer>();
	objectemodel_R->Initialize();
	objectemodel_R->SetModel("cube.obj");
	objectemodel_R->SetTexHandle(objecttexHandle_R);
	objectworldTransform_R.Initialize();
	objectworldTransform_R.translate = { 1.6f,-0.5f,-53.0f };
	objectworldTransform_R.rotate = { 0.0f,0.06f,0.34f };

	//モデルの初期化＆設定(オブジェクト左)
	objectemodel_L = std::make_unique<Object3DPlacer>();
	objectemodel_L->Initialize();
	objectemodel_L->SetModel("cube.obj");
	objectemodel_L->SetTexHandle(objecttexHandle_L);
	objectworldTransform_L.Initialize();
	objectworldTransform_L.translate = { -1.6f,-0.5f,-53.0f };
	objectworldTransform_L.rotate = { 0.0f,-0.06f,-0.34f };

	//モデルの初期化＆設定(オブジェクト左)
	Startmodel_ = std::make_unique<Object3DPlacer>();
	Startmodel_->Initialize();
	Startmodel_->SetModel("cube.obj");
	Startmodel_->SetTexHandle(StarttexHandle_);
	StartworldTransform_.Initialize();
	StartworldTransform_.translate = { 0.0f,-0.87f,-53.36f };
	StartworldTransform_.scale = { 0.5f,0.5f,0.0f };

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	flag = true;
}

void TitleScene::Update() {
	//各種更新処理
	worldTransform.UpdateMatrix();
	TitleworldTransform_.UpdateMatrix();
	objectworldTransform_R.UpdateMatrix();
	objectworldTransform_L.UpdateMatrix();
	StartworldTransform_.UpdateMatrix();
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

	//オブジェクト回転
	objectworldTransform_R.rotate.y -= 0.08f;
	objectworldTransform_L.rotate.y += 0.08f;


	//スプライトに情報を渡す
	sprite_->SetWorldTransform(spriteWorldTransform);
	blackSprite_->SetWorldTransform(spriteWorldTransform);

}

void TitleScene::Draw(){

	//天球の描画
	skydome_->Draw(camera);

	//モデルの描画
	Titlemodel_->Draw(TitleworldTransform_, camera);
	objectemodel_R->Draw(objectworldTransform_R, camera);
	objectemodel_L->Draw(objectworldTransform_L, camera);
	Startmodel_->Draw(StartworldTransform_, camera);


	//スプライトの描画
	//sprite_->Draw();
	blackSprite_->Draw();

	///デバック場面
#ifdef _DEBUG

	ImGui::Begin("WorldTransform");
	if (ImGui::TreeNode("TitleworldTransform_")) {
		ImGui::DragFloat3("translate", &TitleworldTransform_.translate.x, 0.01f, 100, 100);
		ImGui::DragFloat3("rotate", &TitleworldTransform_.rotate.x, 0.01f, -6.28f, 6.28f);
		ImGui::DragFloat3("scale", &TitleworldTransform_.scale.x, 0.01f, 0, 10);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("objectworldTransform_R")) {
		ImGui::DragFloat3("translate", &objectworldTransform_R.translate.x, 0.01f, 100, 100);
		ImGui::DragFloat3("rotate", &objectworldTransform_R.rotate.x, 0.01f, -6.28f, 6.28f);
		ImGui::DragFloat3("scale", &objectworldTransform_R.scale.x, 0.01f, 0, 10);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("objectworldTransform_L")) {
		ImGui::DragFloat3("translate", &objectworldTransform_L.translate.x, 0.01f, 100, 100);
		ImGui::DragFloat3("rotate", &objectworldTransform_L.rotate.x, 0.01f, -6.28f, 6.28f);
		ImGui::DragFloat3("scale", &objectworldTransform_L.scale.x, 0.01f, 0, 10);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("StartworldTransform_")) {
		ImGui::DragFloat3("translate", &StartworldTransform_.translate.x, 0.01f, 100, 100);
		ImGui::DragFloat3("rotate", &StartworldTransform_.rotate.x, 0.01f, 100, 100);
		ImGui::DragFloat3("scale", &StartworldTransform_.scale.x, 0.01f, 100, 100);
		ImGui::TreePop();
	}




	ImGui::End();
	
#endif // _DEBUG



}

void TitleScene::CheckAllCollisions(){
	

}
