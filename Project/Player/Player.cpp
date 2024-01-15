#include "Player.h"

Player::Player(){
}

Player::~Player(){
	for (PlayerBullet* bullet_ : bullets_) {
		delete bullet_;
	}
}

void Player::Initialize(){
	worldTransform_.Initialize();
	viewProjection_.Initialize();

	texHandle_ = TextureManager::Load("resources/uvChecker.png");
	texHandleBullet_= TextureManager::Load("resources/black.png");

	model_.reset(Model::CreateObj("cube.obj"));
	model_->SetTexHandle(texHandle_);



	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
}

void Player::Update(){
	worldTransform_.UpdateMatrix();
	viewProjection_.UpdateMatrix();

	if (input_->PushKey(DIK_RIGHT)) {
		worldTransform_.translate.x += 0.1f;
	}
	else if (input_->PushKey(DIK_LEFT)) {
		worldTransform_.translate.x -= 0.1f;
	}
	if (input_->PushKey(DIK_UP)) {
		worldTransform_.translate.y += 0.1f;
	}
	else if (input_->PushKey(DIK_DOWN)) {
		worldTransform_.translate.y -= 0.1f;
	}

	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotate.y -= 0.02f;
	}
	else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotate.y += 0.02f;
	}

	Attack();
	for(PlayerBullet* bullet_:bullets_){
		bullet_->Update();
	}
}

void Player::Attack(){
	if (input_->PushKey(DIK_SPACE)) {

		
		//弾の生成＆初期化
		PlayerBullet* newBullet = new PlayerBullet();

		newBullet->Initialize(texHandleBullet_,worldTransform_.translate);

		bullets_.push_back(newBullet);
	}
}

void Player::Draw(ViewProjection viewProjection_){
	model_->Draw(worldTransform_, viewProjection_);
	for (PlayerBullet* bullet_ : bullets_) {
		bullet_->Draw(viewProjection_);
	}
}
