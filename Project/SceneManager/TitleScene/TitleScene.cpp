#include "TitleScene.h"
#include <Xinput.h>



TitleScene::TitleScene() {
}

TitleScene::~TitleScene() {
	for (BlockCore* core_ : cores_) {
		delete core_;
	}
}

void TitleScene::Initialize(){
	//ワールド＆カメラの初期化
	worldTransform.Initialize();
	camera.Initialize();
	audio_->Initialize();

	//画像の読み込み
	TitletexHandle_ = TextureManager::Load("resources/7g9a5.png");
	
	StarttexHandle_ = TextureManager::Load("resources/7nu95.png");

	SkydometexHandle_ = TextureManager::Load("resources/1034303-492673.png");
	blacktexHandle_ = TextureManager::Load("resources/black.png");

	coreTexHandle_ = TextureManager::Load("resources/grya.png");

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
	

	//モデルの初期化＆設定(スタートオブジェクト)
	Startmodel_ = std::make_unique<Object3DPlacer>();
	Startmodel_->Initialize();
	Startmodel_->SetModel("cube.obj");
	Startmodel_->SetTexHandle(StarttexHandle_);
	StartworldTransform_.Initialize();

	//モデルの初期化＆設定(エンドオブジェクト右)
	Endrightmodel_ = std::make_unique<Object3DPlacer>();
	Endrightmodel_->Initialize();
	Endrightmodel_->SetModel("cube.obj");
	Endrightmodel_->SetTexHandle(coreTexHandle_);
	EndrightworldTransform_.Initialize();

	//モデルの初期化＆設定(エンドオブジェクト左)
	EndLeftmodel_ = std::make_unique<Object3DPlacer>();
	EndLeftmodel_->Initialize();
	EndLeftmodel_->SetModel("cube.obj");
	EndLeftmodel_->SetTexHandle(coreTexHandle_);
	EndLeftworldTransform_.Initialize();
	
	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	TransitionFlag = true;

	//BGM
	sceneBGM = audio_->SoundLoadWave("resources/dance.wav");
	sceneSE = audio_->SoundLoadWave("resources/EndSE.wav");
	audio_->SoundPlayLoop(sceneBGM);

	//調整項目の追加
	globalVariables_ = GlobalVariables::GatInstance();
	//グループを追加
	GlobalVariables::GatInstance()->CreateGroup(groupName);

	//各項目の調整の追加
	//タイトルのオブジェクト
	globalVariables_->AddItem(groupName, "TitleworldTransform_.translate", TitleworldTransform_.translate);
	globalVariables_->AddItem(groupName, "TitleworldTransform_.scale", TitleworldTransform_.scale);
	//タイトルのスタートオブジェクト
	globalVariables_->AddItem(groupName, "StartworldTransform_.translate", StartworldTransform_.translate);
	globalVariables_->AddItem(groupName, "StartworldTransform_.scale", StartworldTransform_.scale);
	//エンドオブジェクト右(画面遷移)
	globalVariables_->AddItem(groupName, "EndrightworldTransform_.translate", EndrightworldTransform_.translate);
	globalVariables_->AddItem(groupName, "EndrightworldTransform_.scale", EndrightworldTransform_.scale);
	//エンドオブジェクト左(画面遷移)
	globalVariables_->AddItem(groupName, "EndLeftworldTransform_.translate", EndLeftworldTransform_.translate);
	globalVariables_->AddItem(groupName, "EndLeftworldTransform_.scale", EndLeftworldTransform_.scale);

	//エンドオブジェクト右(画面遷移)の移行
	EndrightworldTransform_.translate = globalVariables_->GetVector3Value(groupName, "EndrightworldTransform_.translate");
	EndrightworldTransform_.scale = globalVariables_->GetVector3Value(groupName, "EndrightworldTransform_.scale");
	//エンドオブジェクト左(画面遷移)の移行
	EndLeftworldTransform_.translate = globalVariables_->GetVector3Value(groupName, "EndLeftworldTransform_.translate");
	EndLeftworldTransform_.scale = globalVariables_->GetVector3Value(groupName, "EndLeftworldTransform_.scale");
}

void TitleScene::Update() {
	//各種更新処理
	worldTransform.UpdateMatrix();
	TitleworldTransform_.UpdateMatrix();
	StartworldTransform_.UpdateMatrix();
	EndrightworldTransform_.UpdateMatrix();
	EndLeftworldTransform_.UpdateMatrix();
	camera.UpdateMatrix();
	Sprite::StaticUpdate();

	//jsonのデータのアップロード
	ApplyGlobalVariaBles();

	//スプライトの情報をもらう
	spriteWorldTransform = sprite_->GetWorldTransform();


	//天球の更新
	skydome_->Update();


	//演出開始
	if (input_->PressedKey(DIK_SPACE) && TransitionFlag == true) {
		audio_->SoundPlayStop(sceneBGM);
		audio_->SoundPlayWave(sceneSE);
		TransitionFlag = false;
	}


	//演出
	if (TransitionFlag == false) {
		EndrightworldTransform_.translate.x -= 0.04f;
		EndLeftworldTransform_.translate.x += 0.04f;
	}

	if (EndrightworldTransform_.translate.x<=0.9f&&
		EndLeftworldTransform_.translate.x>=-0.9f) {
		//発生しているブロックの削除
		for (BlockCore* core_ : cores_) {
			core_->SetIsDead(true);
		}
		//次のシーンへ
		sceneNo_ = GAME;
	}




	//rand関数のリセット
	srand((unsigned int)time(NULL));

	//スライドするブロックの設定
	
	if (foolTime >= 20) {
		BlockCore* newCore = new BlockCore();
		// 初期化
		newCore->Initialize(coreTexHandle_);
		newCore->SetIsTitleflag(true);
		randPos_ = { static_cast<float>(rand() % 21) ,
			12.f,
			static_cast<float>(rand() % 61-10)
		};
		newCore->SetWorldPosition(randPos_);
		cores_.push_back(newCore);
		foolTime = 0;
	}
	else {
		foolTime++;
	}

	srand((unsigned int)time(NULL));
	if (foolTime1 >= 30) {
		BlockCore* newCore = new BlockCore();
		// 初期化
		newCore->Initialize(coreTexHandle_);
		newCore->SetIsTitleflag(true);
		randPos_ = { static_cast<float>(rand() % 21 - 20) ,
			12.f,
			static_cast<float>(rand() % 41 - 10)
		};
		newCore->SetWorldPosition(randPos_);
		cores_.push_back(newCore);
		foolTime1 = 0;
	}
	else {
		foolTime1++;
	}


	for (BlockCore* core_ : cores_) {
		core_->Update();
	}


	//デスラグが立つと削除
	cores_.remove_if([](BlockCore* core) {
		if (core->GetIsDead()) {
			delete core;
			return true;
		}
		return false;
		});
}

void TitleScene::Draw(){
	//画面遷移
	Endrightmodel_->Draw(EndrightworldTransform_, camera);
	EndLeftmodel_->Draw(EndLeftworldTransform_, camera);
	//落ちてくるブロック
	for (BlockCore* core_ : cores_) {
		core_->Draw(camera);
	}
	//天球の描画
	skydome_->Draw(camera);

	//モデルの描画
	Startmodel_->Draw(StartworldTransform_, camera);
	Titlemodel_->Draw(TitleworldTransform_, camera);

}

/// <summary>
/// Jsonファイルからデータをアップロード
/// </summary>
void TitleScene::ApplyGlobalVariaBles(){
	//各項目の調整の追加
	//タイトルのオブジェクト
	TitleworldTransform_.translate = globalVariables_->GetVector3Value(groupName, "TitleworldTransform_.translate");
	TitleworldTransform_.scale = globalVariables_->GetVector3Value(groupName, "TitleworldTransform_.scale");
	//タイトルのスタートオブジェクト
	StartworldTransform_.translate = globalVariables_->GetVector3Value(groupName, "StartworldTransform_.translate");
	StartworldTransform_.scale = globalVariables_->GetVector3Value(groupName, "StartworldTransform_.scale");

}
