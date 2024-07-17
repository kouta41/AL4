#include "TitleScene.h"
#include <Xinput.h>


#define CUSTOM_DEADZONE 12000

TitleScene::TitleScene() {
}

TitleScene::~TitleScene() {
}

void TitleScene::Initialize(){
	worldTransform.Initialize();
	camera.Initialize();
	camera.translate = { -2,0,-110 };

	texHandle_ = TextureManager::Load("resources/white.png");
	SkydometexHandle_ = TextureManager::Load("resources/skydome.jpg");

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
}

void TitleScene::Update() {	
	worldTransform.UpdateMatrix();
	camera.UpdateMatrix();
	//天球の更新
	skydome_->Update();

	//プレイヤーの更新
	player_->Update();

	//敵の更新
	enemy_->Update();
	//enemy_->SetPlayer(player_.get());
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

		
}

void TitleScene::Draw(){

	///デバック場面
	ImGui::Begin("Camera");
	if (ImGui::TreeNode("worldTransform")) {
		ImGui::DragFloat3("translate", &camera.translate.x, 0.1f, 100, 100);
		ImGui::DragFloat("rotateX", &camera.rotate.x, 0.1f, 100, 100);
		ImGui::DragFloat("rotateY", &camera.rotate.y, 0.1f, 100, 100);
		ImGui::DragFloat("rotateZ", &camera.rotate.z, 0.1f, 100, 100);

		ImGui::TreePop();
	}
	ImGui::End();
	//プレイヤーの描画
	player_->Draw(camera);
	//敵の描画
	enemy_->Draw(camera);

	//天球の描画
	skydome_->Draw(camera);
}

void TitleScene::CheckAllCollisions(){
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

}
