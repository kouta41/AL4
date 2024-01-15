#include "Player.h"

Player::Player(){
}

Player::~Player(){
}

void Player::Initialize(){
	worldTransform_.Initialize();
	viewProjection_.Initialize();

	texHandle_ = TextureManager::Load("resources/uvChecker.png");
	model_.reset(Model::CreateObj("cube.obj"));
	model_->SetTexHandle(texHandle_);
}

void Player::Update(){
	worldTransform_.UpdateMatrix();
	viewProjection_.UpdateMatrix();

	if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
		worldTransform_.translate.x += 0.1f;
	}
	else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
		worldTransform_.translate.x -= 0.1f;
	}
	if (Input::GetInstance()->PushKey(DIK_UP)) {
		worldTransform_.translate.y += 0.1f;
	}
	else if (Input::GetInstance()->PushKey(DIK_DOWN)) {
		worldTransform_.translate.y -= 0.1f;
	}
}

void Player::Draw(ViewProjection viewProjection_){
	model_->Draw(worldTransform_, viewProjection_);
}
