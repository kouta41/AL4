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

	texHandle_ = TextureManager::Load("resources/white.png");
	ModelManager::LoadObjModel("cube.obj");
	ModelManager::LoadObjModel("skydome.obj");

	//自キャラの生成
	player_ = std::make_unique<Player>();
	// 自キャラの初期化
	player_->Initialize();
	
}

void TitleScene::Update() {	
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


}