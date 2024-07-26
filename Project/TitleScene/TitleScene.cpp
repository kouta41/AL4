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
	SkyboxTex_ = TextureManager::Load("resources/rostock_laage_airport_4k.dds");

	ModelManager::LoadObjModel("cube.obj");
	ModelManager::LoadObjModel("skydome.obj");

	//自キャラの生成
	player_ = std::make_unique<Player>();
	// 自キャラの初期化
	player_->Initialize();


	motion_ = std::make_unique<Motion>();
	motion_->Initialize("sneakWalk.gltf");
	motion_->SetTexHandle(texHandle_);
	worldTransform.translate = { 0,-0.7f,6.0f };

	Skybox_ = std::make_unique<Skybox>();
	Skybox_->Initialize();

	camera.translate={ 0,0,0};


	
}

void TitleScene::Update() {	
	animationTime += 1.0f / 60.0f;

	animationTime = std::fmod(animationTime, motion_->GetAnimation().duration);
	motion_->SetanimationTime(animationTime);

	worldTransform.UpdateMatrix();
	worldTransform1.UpdateMatrix();
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
//	player_->Draw(camera);

	ImGui::Begin("Player");
	if (ImGui::TreeNode("worldTransform")) {
		ImGui::DragFloat3("translate", &worldTransform.translate.x, 0.1f, 100, 100);
		ImGui::DragFloat3("rotate", &worldTransform.rotate.x, 0.01f, -6.28f, 6.28f);
		ImGui::DragFloat3("scale", &worldTransform.scale.x, 0.01f, 0, 10);
		ImGui::TreePop();
	}
	
	ImGui::DragFloat("animationTime", &animationTime);

	ImGui::End();

	ImGui::Begin("Camera");
	if (ImGui::TreeNode("worldTransform")) {
		ImGui::DragFloat3("translate", &camera.translate.x, 0.1f, 100, 100);
		ImGui::DragFloat3("rotate", &camera.rotate.x, 0.01f, -6.28f, 6.28f);
		ImGui::DragFloat3("scale", &camera.scale.x, 0.01f, 0, 10);
		ImGui::TreePop();
	}
	ImGui::End();
	
	
	directionalLightData_ = motion_->GetdirectionalLightData();
	ImGui::Begin("Settings");
		ImGui::DragFloat4("color", &directionalLightData_->color.x, 0.01f, -6.28f, 6.28f);
		ImGui::DragFloat3("direction", &directionalLightData_->direction.x, 0.01f, -6.28f, 6.28f);
		ImGui::DragFloat("intensity", &directionalLightData_->intensity, 0.01f, -6.28f, 6.28f);

	ImGui::End();

	motion_->SetdirectionalLightData(directionalLightData_);


//	matio_1->Draw(worldTransform1, camera);
	motion_->Draw(worldTransform,camera);
//	model_->Draw(worldTransform, camera);

	Skybox_->Draw(worldTransform1, camera, SkyboxTex_);
}