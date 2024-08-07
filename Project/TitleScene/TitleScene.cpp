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
	worldTransform2.Initialize();

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
	motion_->Initialize("simpleSkin.gltf");
	//motion_->Initialize("walk.gltf");

	motion_->SetTexHandle(texHandle_);
	worldTransform.translate = { 0,-0.7f,6.0f };

	Skybox_ = std::make_unique<Skybox>();
	Skybox_->Initialize();

	camera.translate={ 0,0,0};

	model_ = std::make_unique<Object3DPlacer>();
	model_->Initialize();
	model_->SetModel("cube.obj");
	model_->SetTexHandle(texHandle_);

	model_2 = std::make_unique<Object3DPlacer>();
	model_2->Initialize();
	model_2->SetModel("cube.obj");
	model_2->SetTexHandle(texHandle_);
	worldTransform2.translate = { 0,-0.7f,6.0f };

	json_ = std::make_unique<Json>();
	levelData_ = json_->LoadJson("level");
	json_->Adoption(levelData_);
	
}

void TitleScene::Update() {	
	animationTime += 1.0f / 60.0f;

	animationTime = std::fmod(animationTime, motion_->GetAnimation().duration);
	motion_->SetanimationTime(animationTime);

	worldTransform.UpdateMatrix();
	worldTransform1.UpdateMatrix();
	worldTransform2.UpdateMatrix();
	camera.UpdateMatrix();

	json_->Update();
	//camera.worldPos_ = json_->GetCamera().worldPos_;

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

	ImGui::Begin("Camera");
	if (ImGui::TreeNode("transform")) {
		ImGui::DragFloat3("translate", &camera.translate.x, 0.1f, 100, 100);
		ImGui::DragFloat3("rotate", &camera.rotate.x, 0.01f, -6.28f, 6.28f);
		ImGui::TreePop();
	}
	ImGui::End();
	

	ImGui::Begin("player");
	if (ImGui::TreeNode("worldTransform")) {
		ImGui::DragFloat3("translate", &worldTransform.translate.x, 0.1f, 100, 100);
		ImGui::DragFloat3("rotate", &worldTransform.rotate.x, 0.01f, -6.28f, 6.28f);
		ImGui::DragFloat3("scale", &worldTransform.scale.x, 0.01f, 0, 10);
		ImGui::TreePop();
	}

	ImGui::DragFloat("animationTime", &animationTime, 0.01f);
	ImGui::End();


//	json_->Draw(camera);


//	matio_1->Draw(worldTransform1, camera);

	//motion_->Draw(worldTransform,camera);

	//Skybox_->Draw(worldTransform1, camera, SkyboxTex_);

	model_->Draw(worldTransform, camera);


	model_2->Draw(worldTransform2, camera);

}