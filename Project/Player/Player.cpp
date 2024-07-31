#include "Player.h"

Player::Player(){
}

Player::~Player(){
	for (PlayerCore* core_ : cores_) {
		delete core_;
	}
	for (PlayerCrust* crust_ : crusts_) {
		delete crust_;
	}
}

void Player::Initialize() {
	worldTransform_.Initialize();
	viewProjection_.Initialize();

	coreTexHandle_ = TextureManager::Load("resources/cube.jpg");
	crustTexHandle_= TextureManager::Load("resources/uvChecker.png");


	// 7×7のプレイヤーのデータ
	playerLocation_ =
	{
		{0,0,0,0,0,0,0},
		{0,2,1,1,1,2,0},
		{0,2,2,2,2,2,0},
		{0,2,2,1,2,2,0},
		{0,2,2,2,2,2,0},
		{0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0},
	};

	for (int i = 0; i < MAX_PLAYER_CHIPS; ++i) {
		for (int j = 0; j < MAX_PLAYER_CHIPS; ++j) {
			if (playerLocation_[i][j] == CORE) {
				PlayerCore* newCore = new PlayerCore();
				// 初期化
				newCore->Initialize(coreTexHandle_);
				newCore->SetWorldPosition(
					{ worldTransform_.translate.x + (float(j * 2.1 - MAX_PLAYER_CHIPS - 1)),
					worldTransform_.translate.y - (float(i * 2.1 - MAX_PLAYER_CHIPS - 1)),
					worldTransform_.translate.z });
				cores_.push_back(newCore);
			}
			if (playerLocation_[i][j] == CRUST) {
				PlayerCrust* newCrust = new PlayerCrust();
				//初期化
				newCrust->Initialize(crustTexHandle_);
				newCrust->SetWorldPosition(
					{ worldTransform_.translate.x + (float(j * 2.1 - MAX_PLAYER_CHIPS - 1)),
					worldTransform_.translate.y - (float(i * 2.1 - MAX_PLAYER_CHIPS - 1)),
					worldTransform_.translate.z });
				crusts_.push_back(newCrust);
			}
		}
	}


	//衝突属性を設定
	SetcollisiionAttribute_(kCollitionAttributePlayer);
	//衝突対象を自分以外の属性以外に設定
	SetCollisionMask_(~kCollitionAttributePlayer);

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();


	ModelManager::LoadObjModel("cube.obj");
	
}

void Player::Update(){
	worldTransform_.UpdateMatrix();
	viewProjection_.UpdateMatrix();

	velocity_ = { 0,0,0 };
	if (input_->PushKey(DIK_RIGHT)) {
		velocity_.x += 0.3f;
	}


	if (isAstop_ == false && input_->PushKey(DIK_LEFT)) {
		velocity_.x -= 0.3f;
	}


	if (input_->PushKey(DIK_UP)) {
		velocity_.y += 0.3f;
	}
	if (input_->PushKey(DIK_DOWN)) {
		velocity_.y -= 0.3f;
	}

	for (PlayerCore* core_ : cores_) {
		core_->Update(velocity_);
	}
	for (PlayerCrust* crust_ : crusts_) {
		crust_->Update(velocity_);
	}


	/*
	
	プレイヤーの移動制限は{-30~30,-15~15,0}
	
	*/


	//cores_.remove_if([](PlayerCore* core) {
	//	if (core->isAstop()) {
	//		this->isAstop_ = false;
	//		return true;
	//	}
	//	return false;
	//	});




	//デスラグが立つと削除
	cores_.remove_if([](PlayerCore* core) {
		if (core->IsDead()) {
			delete core;
			return true;
		}
		return false;
		});
	crusts_.remove_if([](PlayerCrust* crust) {
		if (crust->IsDead()) {
			delete crust;
			return true;
		}
		return false;
		});

}



void Player::Draw(CameraRole viewProjection_){


	for (PlayerCore* core_ : cores_) {
		core_->Draw(viewProjection_);
	}
	for (PlayerCrust* crust_ : crusts_) {
		crust_->Draw(viewProjection_);
	}

}


void Player::OnCollision(){
}

Vector3 Player::GetWorldPosition(){
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld.m[3][0];
	worldPos.y = worldTransform_.matWorld.m[3][1];
	worldPos.z = worldTransform_.matWorld.m[3][2];

	return worldPos;
}

Vector3 Player::GetPlayerCoreWorldPosition()
{
	Vector3 worldPos;

	for (PlayerCore* core_ : cores_) {
		worldPos = core_->GetWorldPosition();
	}
	return worldPos;
}

