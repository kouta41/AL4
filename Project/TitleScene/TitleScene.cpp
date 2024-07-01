#include "TitleScene.h"
#include <Xinput.h>


#define CUSTOM_DEADZONE 12000

TitleScene::TitleScene() {
}

TitleScene::~TitleScene() {
}

void TitleScene::Initialize(){
	worldTransform.Initialize();
	worldTransform1.Initialize();
	camera.Initialize();

	texHandle_ = TextureManager::Load("resources/white.png");
	ModelManager::LoadObjModel("cube.obj");
	ModelManager::LoadObjModel("skydome.obj");

	//自キャラの生成
	player_ = std::make_unique<Player>();
	// 自キャラの初期化
	player_->Initialize();

	//SkyboxTex_ = TextureManager::Load("rostock_laage_airport_4k.dds");

	motion_ = std::make_unique<Motion>();
	motion_->Initialize("sneakWalk.gltf");
	motion_->SetTexHandle(texHandle_);
	worldTransform.translate = { 2.0f,0.0f,-50.0f };


	//Skybox_ = std::make_unique<Skybox>();
	//Skybox_->Initialize();



	
}

void TitleScene::Update() {	
	animationTime += 1.0f / 60.0f;
	animationTime1 += 1.0f / 60.0f;

	animationTime = std::fmod(animationTime, animation.duration);
	motion_->SetanimationTime(animationTime);

	//animationTime1 = std::fmod(animationTime1, animation_1.duration);
	//matio_1->SetanimationTime(animationTime1)*/;

	worldTransform1.UpdateMatrix();
	worldTransform.UpdateMatrix();
	camera.UpdateMatrix();

	//プレイヤーの更新
	player_->Update();

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

	//プレイヤーの描画
	player_->Draw(camera);

	ImGui::Begin("Player");
	if (ImGui::TreeNode("worldTransform")) {
		ImGui::DragFloat3("translate", &worldTransform.translate.x, 0.1f, 100, 100);
		ImGui::DragFloat3("rotate", &worldTransform.rotate.x, 0.01f, -6.28f, 6.28f);
		ImGui::DragFloat3("scale", &worldTransform.scale.x, 0.01f, 0, 10);
		ImGui::TreePop();
	}
	
	ImGui::DragFloat("animationTime", &animationTime);
	ImGui::DragFloat("animationTime1", &animationTime1);

	ImGui::End();



//	matio_1->Draw(worldTransform1, camera);
	motion_->Draw(worldTransform,camera);
//	model_->Draw(worldTransform, camera);

//	Skybox_->Draw(worldTransform, camera, texHandle_);
}