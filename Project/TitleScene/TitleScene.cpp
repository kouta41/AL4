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
	audio_->Initialize();

	//画像の読み込み
	TitletexHandle_ = TextureManager::Load("resources/7g9a5.png");
	objecttexHandle_R = TextureManager::Load("resources/81c9l.png");
	objecttexHandle_L = TextureManager::Load("resources/w14s8.png");
	StarttexHandle_ = TextureManager::Load("resources/7nu95.png");

	SkydometexHandle_ = TextureManager::Load("resources/rainbow.jpg");
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
	TitleworldTransform_.translate = { 0.0f,0.6f,-50.75f };
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

	//モデルの初期化＆設定(スタートオブジェクト)
	Startmodel_ = std::make_unique<Object3DPlacer>();
	Startmodel_->Initialize();
	Startmodel_->SetModel("cube.obj");
	Startmodel_->SetTexHandle(StarttexHandle_);
	StartworldTransform_.Initialize();
	StartworldTransform_.translate = { 0.0f,-0.87f,-53.36f };
	StartworldTransform_.scale = { 0.5f,0.5f,0.0f };

	//モデルの初期化＆設定(エンドオブジェクト右)
	Endrightmodel_ = std::make_unique<Object3DPlacer>();
	Endrightmodel_->Initialize();
	Endrightmodel_->SetModel("cube.obj");
	Endrightmodel_->SetTexHandle(blacktexHandle_);
	EndrightworldTransform_.Initialize();
	EndrightworldTransform_.translate = { 2.63f,-0.0f,-56.0f };
	EndrightworldTransform_.scale = { 1.0f,1.0f,0.0f };

	//モデルの初期化＆設定(エンドオブジェクト左)
	EndLeftmodel_ = std::make_unique<Object3DPlacer>();
	EndLeftmodel_->Initialize();
	EndLeftmodel_->SetModel("cube.obj");
	EndLeftmodel_->SetTexHandle(blacktexHandle_);
	EndLeftworldTransform_.Initialize();
	EndLeftworldTransform_.translate = { -2.63f,-0.0f,-56.0f };
	EndLeftworldTransform_.scale = { 1.0f,1.0f,0.0f };

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	flag = true;

	//BGM
	sceneBGM = audio_->SoundLoadWave("resources/dance.wav");
	sceneSE = audio_->SoundLoadWave("resources/EndSE.wav");
	audio_->SoundPlayLoop(sceneBGM);

}

void TitleScene::Update() {
	//各種更新処理
	worldTransform.UpdateMatrix();
	TitleworldTransform_.UpdateMatrix();
	objectworldTransform_R.UpdateMatrix();
	objectworldTransform_L.UpdateMatrix();
	StartworldTransform_.UpdateMatrix();
	EndrightworldTransform_.UpdateMatrix();
	EndLeftworldTransform_.UpdateMatrix();
	camera.UpdateMatrix();
	Sprite::StaticUpdate();

	//スプライトの情報をもらう
	spriteWorldTransform = sprite_->GetWorldTransform();


	//天球の更新
	skydome_->Update();


	//演出開始
	if (input_->PressedKey(DIK_SPACE)) {
		audio_->SoundPlayStop(sceneBGM);
		audio_->SoundPlayWave(sceneSE);
		flag = false;
	}


	//演出
	if (flag == false) {
		EndrightworldTransform_.translate.x -= 0.04f;
		EndLeftworldTransform_.translate.x += 0.04f;
	}

	if (EndrightworldTransform_.translate.x<=0.9f&&
		EndLeftworldTransform_.translate.x>=-0.9f) {
		//次のシーンへ
		sceneNo_ = SELECT;

	}

	//オブジェクト回転
	objectworldTransform_R.rotate.y -= 0.08f;
	objectworldTransform_L.rotate.y += 0.08f;


	//
	srand((unsigned int)time(NULL));
	//TitleworldTransform_.translate = { float(rand()% 5 - 2)/100,float(rand() % 5 - 1)/10 ,-50.75f };
	if (TitleworldTransform_.scale.y<=0) {
		speed *= -1;
	}
	else if (TitleworldTransform_.scale.y >= 1.6) {
		speed *= -1;
	}

	TitleworldTransform_.scale.y += speed;
}

void TitleScene::Draw(){
	//画面遷移
	Endrightmodel_->Draw(EndrightworldTransform_, camera);
	EndLeftmodel_->Draw(EndLeftworldTransform_, camera);

	//天球の描画
	skydome_->Draw(camera);

	//モデルの描画
	Startmodel_->Draw(StartworldTransform_, camera);
	Titlemodel_->Draw(TitleworldTransform_, camera);
	objectemodel_R->Draw(objectworldTransform_R, camera);
	objectemodel_L->Draw(objectworldTransform_L, camera);


	//スプライトの描画
	//sprite_->Draw();
	//blackSprite_->Draw();

	///デバック場面
#ifdef _DEBUG

	ImGui::Begin("WorldTransform");
	if (ImGui::TreeNode("EndrightworldTransform_")) {
		ImGui::DragFloat3("translate", &EndrightworldTransform_.translate.x, 0.01f, 100, 100);
		ImGui::DragFloat3("rotate", &EndrightworldTransform_.rotate.x, 0.01f, -6.28f, 6.28f);
		ImGui::DragFloat3("scale", &EndrightworldTransform_.scale.x, 0.01f, 0, 10);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("EndLeftworldTransform_")) {
		ImGui::DragFloat3("translate", &EndLeftworldTransform_.translate.x, 0.01f, 100, 100);
		ImGui::DragFloat3("rotate", &EndLeftworldTransform_.rotate.x, 0.01f, -6.28f, 6.28f);
		ImGui::DragFloat3("scale", &EndLeftworldTransform_.scale.x, 0.01f, 0, 10);
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
	if (ImGui::TreeNode("Color_R")) {
		ImGui::DragFloat("Color_R", &Color_R, 0.01f, 100, 100);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Color_L")) {
		ImGui::DragFloat("Color_L", &Color_L, 0.01f, 100, 100);
		ImGui::TreePop();
	}


	ImGui::End();
	
#endif // _DEBUG



}

void TitleScene::CheckAllCollisions(){
	

}
