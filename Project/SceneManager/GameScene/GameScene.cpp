#include "GameScene.h"
#include <Xinput.h>


#define CUSTOM_DEADZONE 12000


GameScene::GameScene() {}

GameScene::~GameScene() {
	
}

void GameScene::Initialize(){
	worldTransform.Initialize();
	camera.Initialize();
	camera.translate = { 0,0,-110 };

	texHandle_ = TextureManager::Load("resources/white.png");
	SkydometexHandle_ = TextureManager::Load("resources/skydome.jpg");
	blacktexHandle_ = TextureManager::Load("resources/black.png");
	StarttexHandle_ = TextureManager::Load("resources/GO.png");


	ModelManager::LoadObjModel("cube.obj");
	ModelManager::LoadObjModel("skydome.obj");

	// 当たり判定のインスタンスを生成
	collisionManager_ = std::make_unique<CollisionManager>();

	//自キャラの生成
	player_ = std::make_unique<Player>();
	// 自キャラの初期化
	player_->Initialize(collisionManager_.get());


	//敵キャラの生成
	//enemy_ = std::make_unique<Enemy>();
	// 敵キャラの初期化
	//enemy_->Initialize();


	//天球の生成
	skydome_ = std::make_unique<Skydome>();
	//天球の初期化
	skydome_->Initialize(SkydometexHandle_);


	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();


	gameTime = 0;


	worldTransform_1.Initialize();
	worldTransform_2.Initialize();
	worldTransform_3.Initialize();
	worldTransform_4.Initialize();

	
	//モデルの初期化＆設定(エンドオブジェクト右)
	Endrightmodel_ = std::make_unique<Object3DPlacer>();
	Endrightmodel_->Initialize();
	Endrightmodel_->SetModel("cube.obj");
	Endrightmodel_->SetTexHandle(blacktexHandle_);
	EndrightworldTransform_.Initialize();
	EndrightworldTransform_.translate = { 1.0f,-0.0f,-105.7f };
	EndrightworldTransform_.scale = { 1.0f,1.0f,0.0f };

	//モデルの初期化＆設定(エンドオブジェクト左)
	EndLeftmodel_ = std::make_unique<Object3DPlacer>();
	EndLeftmodel_->Initialize();
	EndLeftmodel_->SetModel("cube.obj");
	EndLeftmodel_->SetTexHandle(blacktexHandle_);
	EndLeftworldTransform_.Initialize();
	EndLeftworldTransform_.translate = { -1.0f,-0.0f,-105.7f };
	EndLeftworldTransform_.scale = { 1.0f,1.0f,0.0f };

	//モデルの初期化＆設定(スタートオブジェクト)
	Startmodel_ = std::make_unique<Object3DPlacer>();
	Startmodel_->Initialize();
	Startmodel_->SetModel("cube.obj");
	Startmodel_->SetTexHandle(StarttexHandle_);
	StartworldTransform_.Initialize();
	StartworldTransform_.translate = { -2.0f,-0.87f,-240.0f };
	StartworldTransform_.scale = { 1.0f,1.0f,0.0f };

	flag = true;
	count_ = 0;
	cameraflag1 = true;
	cameraflag2 = true;

	frame1 = 0.0f;
	frame2 = 0.0f;

	endframe1 = 120.0f;
	endframe2 = 60.0f;
}

void GameScene::Update(){
	worldTransform.UpdateMatrix();
	worldTransform_1.UpdateMatrix();
	worldTransform_2.UpdateMatrix();
	worldTransform_3.UpdateMatrix();
	worldTransform_4.UpdateMatrix();
	EndrightworldTransform_.UpdateMatrix();
	EndLeftworldTransform_.UpdateMatrix();
	StartworldTransform_.UpdateMatrix();

	camera.UpdateMatrix();

	

	//天球の更新
	skydome_->Update();


	if (flag == false) {
		if (count_ < 16) {

			//プレイヤーの更新
			player_->Update();

			//敵の更新
			//enemy_->Update();
			//enemy_->SetPlayerCorepos(player_->GetPlayerCoreWorldPosition());

			// 当たり判定
			collisionManager_->CheckAllCollisions();

			XINPUT_STATE joyState{};
			if (Input::GetInstance()->GetJoystickState(joyState)) {
				if ((joyState.Gamepad.sThumbLX < CUSTOM_DEADZONE && joyState.Gamepad.sThumbLX > -CUSTOM_DEADZONE) && (joyState.Gamepad.sThumbLY < CUSTOM_DEADZONE && joyState.Gamepad.sThumbLY > -CUSTOM_DEADZONE)) {
					joyState.Gamepad.sThumbLX = 0;
					joyState.Gamepad.sThumbLY = 0;
					worldTransform.translate.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * 0.1f;
					worldTransform.translate.z += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * 0.1f;
				}
				else {
					worldTransform.translate.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * 0.1f;
					worldTransform.translate.z += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * 0.1f;
				}

			}

			EndrightworldTransform_.translate.x += 0.04f;
			EndLeftworldTransform_.translate.x -= 0.04f;
		}
	}

	
	if (player_->GetClearCount_() == 4.0f) {
		sceneNo_ = END;
	}


	//演出
	if (flag == true) {
		EndrightworldTransform_.translate.x += 0.04f;
		EndLeftworldTransform_.translate.x -= 0.04f;
	}

	if (EndrightworldTransform_.translate.x >= 2.63f &&
		EndLeftworldTransform_.translate.x <= -2.63f) {
			flag = false;
	}

	StartworldTransform_.translate.z += 3;
	if (flag == false) {
		StartworldTransform_.translate.y++;
		if (input_->PushKey(DIK_B)) {
			sceneNo_ = END;
		}
	}

	if (count_ == 16) {
		camera.translate = { posA.x,posA.y ,posA.z-20 };
		cameraRotate = camera.rotate;
		cameraPosA= { posA.x,posA.y ,posA.z - 20 };
		cameraflag1 = false;
		count_++;
	}

	if (cameraflag1 == false) {
		frame1++;
		camera.translate.z = cameraPosA.z + (cameraPosA.z - 60 - cameraPosA.z) * easeoutCubic(frame1/ endframe1);
		if (frame1 == endframe1) {
			cameraflag2 = false;
			cameraflag1 = true;
			cameraPosA = camera.translate;
		}
	}

	if (cameraflag2 == false) {
		frame2++;
		camera.translate.z=cameraPosA.z+(-0.5f-cameraPosA.z)* easeInQuart(frame2 / endframe2);
		if (frame2 == endframe2) {
			cameraflag2 = true;
			sceneNo_ = END;
		}
	}
}

void GameScene::Draw(){
#ifdef _DEBUG
	ImGui::Begin("Camera");
	if (ImGui::TreeNode("camera")) {
		ImGui::DragFloat3("translate", &camera.translate.x, 0.1f, 100, 100);
		ImGui::DragFloat3("rotate", &camera.rotate.x, 0.1f, 100, 100);


		ImGui::TreePop();
	}
	
	ImGui::End();
#endif // RELEASE


	//モデルの描画
	//Startmodel_->Draw(StartworldTransform_, camera);
	//画面遷移
	Endrightmodel_->Draw(EndrightworldTransform_, camera);
	EndLeftmodel_->Draw(EndLeftworldTransform_, camera);

	//プレイヤーの描画
	player_->Draw(camera);
	//敵の描画
	//enemy_->Draw(camera);

	
	
	//天球の描画
	//skydome_->Draw(camera);



}


