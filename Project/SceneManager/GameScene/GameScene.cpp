#include "GameScene.h"
#include <Xinput.h>


#define CUSTOM_DEADZONE 12000


GameScene::GameScene() {}

GameScene::~GameScene() {
	
}

void GameScene::Initialize(){
	worldTransform.Initialize();
	camera.Initialize();
	camera.translate = { -2,0,-110 };

	texHandle_ = TextureManager::Load("resources/white.png");
	SkydometexHandle_ = TextureManager::Load("resources/skydome.jpg");
	blacktexHandle_ = TextureManager::Load("resources/black.png");
	StarttexHandle_ = TextureManager::Load("resources/GO.png");


	ModelManager::LoadObjModel("cube.obj");
	ModelManager::LoadObjModel("skydome.obj");

	//自キャラの生成
	player_ = std::make_unique<Player>();
	// 自キャラの初期化
	player_->Initialize();


	//敵キャラの生成
	enemy_ = std::make_unique<Enemy>();
	// 敵キャラの初期化
	enemy_->Initialize();


	//天球の生成
	skydome_ = std::make_unique<Skydome>();
	//天球の初期化
	skydome_->Initialize(SkydometexHandle_);

	//当たり判定の初期化
	collisionManager_ = new CollisionManager();

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();


	gameTime = 0;


	worldTransform_1.Initialize();
	worldTransform_2.Initialize();
	worldTransform_3.Initialize();
	worldTransform_4.Initialize();

	model_1 = std::make_unique<Object3DPlacer>();
	model_1->Initialize();
	model_1->SetModel("cube.obj");
	model_1->SetTexHandle(texHandle_);
	worldTransform_1.translate = { 30,0,0 };
	worldTransform_1.scale = { 1,15,1 };

	model_2 = std::make_unique<Object3DPlacer>();
	model_2->Initialize();
	model_2->SetModel("cube.obj");
	model_2->SetTexHandle(texHandle_);
	worldTransform_2.translate = { -30,0,0 };
	worldTransform_2 .scale = { 1,15,1 };


	model_3 = std::make_unique<Object3DPlacer>();
	model_3->Initialize();
	model_3->SetModel("cube.obj");
	model_3->SetTexHandle(texHandle_);
	worldTransform_3.translate = { 0,-15,0 };
	worldTransform_3.scale = { 30,1,1 };

	model_4 = std::make_unique<Object3DPlacer>();
	model_4->Initialize();
	model_4->SetModel("cube.obj");
	model_4->SetTexHandle(texHandle_);
	worldTransform_4.translate = { 0,15,0 };
	worldTransform_4.scale = { 30,1,1, };

	//モデルの初期化＆設定(エンドオブジェクト右)
	Endrightmodel_ = std::make_unique<Object3DPlacer>();
	Endrightmodel_->Initialize();
	Endrightmodel_->SetModel("cube.obj");
	Endrightmodel_->SetTexHandle(blacktexHandle_);
	EndrightworldTransform_.Initialize();
	EndrightworldTransform_.translate = { -1.0f,-0.0f,-105.7f };
	EndrightworldTransform_.scale = { 1.0f,1.0f,0.0f };

	//モデルの初期化＆設定(エンドオブジェクト左)
	EndLeftmodel_ = std::make_unique<Object3DPlacer>();
	EndLeftmodel_->Initialize();
	EndLeftmodel_->SetModel("cube.obj");
	EndLeftmodel_->SetTexHandle(blacktexHandle_);
	EndLeftworldTransform_.Initialize();
	EndLeftworldTransform_.translate = { -3.0f,-0.0f,-105.7f };
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
			enemy_->Update();
			enemy_->SetPlayerCorepos(player_->GetPlayerCoreWorldPosition());

			//当たり判定
			CheckAllCollisions();


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

	gameTime++;
	if (gameTime >= 60 * 20) {
		//sceneNo_ = END;
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
		if (input_->PushKey(DIK_SPACE)) {
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
	///デバック場面
	ImGui::Begin("Camera");
	if (ImGui::TreeNode("worldTransform")) {
		ImGui::DragFloat3("translate", &camera.translate.x, 0.1f, 100, 100);
		ImGui::DragFloat("rotateX", &camera.rotate.x, 0.1f, 100, 100);
		ImGui::DragFloat("rotateY", &camera.rotate.y, 0.1f, 100, 100);
		ImGui::DragFloat("rotateZ", &camera.rotate.z, 0.1f, 100, 100);


		ImGui::TreePop();
	}

	ImGui::DragInt("Count", &count_, 0.01f, 0, 10);
	ImGui::DragFloat3("pos", &posA.x, 0.1f, 100, 100);

	ImGui::End();

	
#endif // RELEASE


	//モデルの描画
	Startmodel_->Draw(StartworldTransform_, camera);
	//画面遷移
	Endrightmodel_->Draw(EndrightworldTransform_, camera);
	EndLeftmodel_->Draw(EndLeftworldTransform_, camera);

	//プレイヤーの描画
	player_->Draw(camera);
	//敵の描画
	enemy_->Draw(camera);

	model_1->Draw(worldTransform_1, camera);
	model_2->Draw(worldTransform_2, camera);
	model_3->Draw(worldTransform_3, camera);
	model_4->Draw(worldTransform_4, camera);
	
	//天球の描画
	//skydome_->Draw(camera);



}

void GameScene::CheckAllCollisions(){
	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetEnemyBullsts();
	for (EnemyBullet* bullet : enemyBullets) {
		collisionManager_->AddCollider(bullet);
	}
	// プレイヤーの外殻リストの取得
	const std::list<PlayerCore*>& playerCores = player_->GetPlayerCores();
	for (PlayerCore* cores : playerCores) {
		collisionManager_->AddCollider(cores);
	}
	// 敵弾リストの取得
	const std::list<PlayerCrust*>& playerCrusts = player_->GetPlayerCrusts();
	for (PlayerCrust* crusts : playerCrusts) {
		collisionManager_->AddCollider(crusts);
	}

	collisionManager_->CheckAllCollisions();
	collisionManager_->ClearCollider();

	count_ = collisionManager_->GethitCount();
	posA = collisionManager_->GetposA();
}
