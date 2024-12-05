#include "ScreenTransition.h"

ScreenTransition::ScreenTransition(){
}

ScreenTransition::~ScreenTransition(){
}

void ScreenTransition::Initialize(){
	//サウンドの初期化
	audio_->Initialize();

	//画像の読み込み
	blacktexHandle_ = TextureManager::Load("resources/black.png");

	//モデルデータの読み込み
	ModelManager::LoadObjModel("cube.obj");


	//画面遷移_右
	rightmodel_ = std::make_unique<Object3DPlacer>();
	rightmodel_->Initialize();
	rightmodel_->SetModel("cube.obj");
	rightmodel_->SetTexHandle(blacktexHandle_);
	worldTransform_R.Initialize();
	

	//画面遷移_右
	Leftmodel_ = std::make_unique<Object3DPlacer>();
	Leftmodel_->Initialize();
	Leftmodel_->SetModel("cube.obj");
	Leftmodel_->SetTexHandle(blacktexHandle_);
	worldTransform_L.Initialize();
	

	//SE
	sceneSE = audio_->SoundLoadWave("resources/EndSE.wav");
}

void ScreenTransition::SetStartWorld(){
	worldTransform_R.translate = { 2.63f,-0.0f,-56.0f };
	worldTransform_L.translate = { -2.63f,-0.0f,-56.0f };
}

void ScreenTransition::SetEndWorld(){
	worldTransform_R.translate = { 2.63f,-0.0f,-56.0f };
	worldTransform_L.translate = { -2.63f,-0.0f,-56.0f };
}

void ScreenTransition::Update(){
	//ワールド座標の更新
	worldTransform_L.UpdateMatrix();
	worldTransform_R.UpdateMatrix();
}

void ScreenTransition::StartMove(){
	worldTransform_R.translate.x += 0.04f;
	worldTransform_L.translate.x -= 0.04f;
}

void ScreenTransition::EndMove(){
	worldTransform_R.translate.x -= 0.04f;
	worldTransform_L.translate.x += 0.04f;
}

void ScreenTransition::Draw(CameraRole camera){
	//画面遷移
	rightmodel_->Draw(worldTransform_R, camera);
	Leftmodel_->Draw(worldTransform_L, camera);
}
