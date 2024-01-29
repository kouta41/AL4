#include "Player.h"

Player::Player(){
}

Player::~Player(){
	for (PlayerBullet* bullet_ : bullets_) {
		delete bullet_;
	}
}

void Player::Initialize() {
	worldTransform_.Initialize();
	viewProjection_.Initialize();

	texHandle_ = TextureManager::Load("resources/uvChecker.png");
	texHandleBullet_ = TextureManager::Load("resources/black.png");

	model_.reset(Model::CreateObj("cube.obj"));
	model_->SetTexHandle(texHandle_);

	worldTransform_.translate = {0, -5, 30};
	//衝突属性を設定
	SetcollisiionAttribute_(kCollitionAttributePlayer);
	//衝突対象を自分以外の属性以外に設定
	SetCollisionMask_(~kCollitionAttributePlayer);

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
}

void Player::Update(){
	worldTransform_.UpdateMatrix();
	viewProjection_.UpdateMatrix();

	//デスフラグの立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
		});

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

	if (input_->PushKey(DIK_W)) {
		worldTransform_.translate.z += 0.1f;
	}
	else if (input_->PushKey(DIK_S)) {
		worldTransform_.translate.z -= 0.1f;
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
	if (input_->PushKeyPressed(DIK_SPACE)) {
		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);
		
		//速度ベクトルを自機の向きに合わせて回転させる
		velocity = TransformNormal(velocity, worldTransform_.matWorld);

		//弾の生成＆初期化
		PlayerBullet* newBullet = new PlayerBullet();

		newBullet->Initialize(texHandleBullet_,worldTransform_.translate, velocity);

		bullets_.push_back(newBullet);
	}
}

void Player::Draw(ViewProjection viewProjection_){
	model_->Draw(worldTransform_, viewProjection_);
	for (PlayerBullet* bullet_ : bullets_) {
		bullet_->Draw(viewProjection_);
	}
	ImGui::Begin("Player");
	if (ImGui::TreeNode("worldTransform")) {
		ImGui::DragFloat3("translate", &worldTransform_.translate.x, 0.1f, 100, 100);
		ImGui::DragFloat3("rotate", &worldTransform_.rotate.x, 0.01f, -6.28f, 6.28f);
		ImGui::DragFloat3("scale", &worldTransform_.scale.x, 0.01f, 0, 10);
		ImGui::TreePop();
	}
	ImGui::End();
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

void Player::setParent(const WorldTransform* parent){
	//親子関係を結ぶ
	worldTransform_.parent = parent;
}
