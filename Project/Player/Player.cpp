#include "Player.h"
#include <Skybox.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <cassert>



Player::~Player() {

}

void Player::Initialize() {
	// モデルの読み込み
	ModelManager::LoadObjModel("cube.obj"); 
	texHandle_ = TextureManager::Load("resources/enemy.png");

	model_ = std::make_unique<Object3DPlacer>();
	model_->Initialize();
	model_->SetModel("cube.obj");
	model_->SetTexHandle(texHandle_);

	worldTransform_.Initialize();
	// 初期位置
	worldTransform_.translate = { 0,0,0 };
	// 右向きに設定
	worldTransform_.rotate = { 0,(float)M_PI / 2, 0 };

	worldTransform_.UpdateMatrix();

	// 速度ベクトル
	velocity_ = { 0,0,kSpeed };
	// 自機の向きに合わせ、回転
	velocity_ = TransformNormal(velocity_, worldTransform_.matWorld);
	// 加速度
	acceleration_ = { 0,-0.04f,0 };
	// ジャンプの初速
	initialVel_ = 0.5f;

	// 進む方向を逆にする信号
	inverseVelSignal_ = false;


	// ジャンプしたか
	isJump_ = false;
	// 生きてるよ
	isAlive_ = true;


	// 当たり判定の形状を設定
	SetCollisionPrimitive_(kCollisionAABB);
	// 衝突属性を設定
	SetCollisionAttribute_(kAttributePlayer);

	AABB aabb = {
	{-0.8f,-1.0f,-0.8f},
	{0.8f,1.0f,0.8f}
	};
	SetAABB_(aabb);

	hitCounter_ = 0;
}


void Player::Update() {
	// 重力を速度に足す
	velocity_.y += acceleration_.y;

	// 床の判定
	if (worldTransform_.translate.y <= -12) {
		worldTransform_.translate.y = -12;
		velocity_.y = 0;
	}

	if (worldTransform_.translate.x <= -12 || worldTransform_.translate.x >= 12) {
		inverseVelSignal_ = true;
	}

	/// ふるまい
	// 初期化
	B_Init();
	// 更新処理
	B_Update();

	// 移動に関する処理
	Move();

	// ImGui
	AdjustmentParameter();

	// ワールド座標の更新
	worldTransform_.UpdateMatrix();
}

void Player::Draw(const CameraRole& camera_) {
	model_->Draw(worldTransform_, camera_);
}

void Player::OnCollision(Collider* collider) {
	float theta = atan2(worldTransform_.translate.y - collider->GetWorldPosition().y, worldTransform_.translate.x - collider->GetWorldPosition().x);

	// 上
	if (theta <= -(M_PI / 3.5f) && theta >= -M_PI + (M_PI / 3.5f)) {
		isAlive_ = false;
		worldTransform_.UpdateMatrix();
	}
	// 下
	if (theta >= (M_PI / 5.0f) && theta <= M_PI - (M_PI / 5)) {
		float extrusion = (-GetAABB_().min.y + collider->GetAABB_().max.y) - (worldTransform_.translate.y - collider->GetWorldPosition().y);
		worldTransform_.translate.y += extrusion;
		worldTransform_.UpdateMatrix();	
		if (hitCounter_ <= 0) {
			isJump_ = false;
			behaviorRequest_ = Behavior::kLanding;
		}
		velocity_.y = 0;
		hitCounter_++;
	}

	// 左
	if (theta < M_PI / 10.0f && theta > -(M_PI / 10.0f)) {
		float extrusion = (-GetAABB_().min.x + collider->GetAABB_().max.x) - (worldTransform_.translate.x - collider->GetWorldPosition().x);
		worldTransform_.translate.x += extrusion;
		worldTransform_.UpdateMatrix();
		if (!collider->GetIsTopHitAABB_()) {
			if (collider->GetIsBottomHitAABB_()) {
				isJump_ = true;
			}
		}
		else {
			inverseVelSignal_ = true;
		}
	}
	// 右
	if (theta > M_PI - (M_PI / 10.0f) || theta < -M_PI + (M_PI / 10.0f)) {
		float extrusion = (GetAABB_().max.x + (-collider->GetAABB_().min.x)) - (collider->GetWorldPosition().x - worldTransform_.translate.x);
		worldTransform_.translate.x -= extrusion;
		worldTransform_.UpdateMatrix();
		if (!collider->GetIsTopHitAABB_()) {
			if (collider->GetIsBottomHitAABB_()) {
				isJump_ = true;
			}
		}
		else {
			inverseVelSignal_ = true;
		}
	}
}


Vector3 Player::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld.m[3][0];
	worldPos.y = worldTransform_.matWorld.m[3][1];
	worldPos.z = worldTransform_.matWorld.m[3][2];

	return worldPos;
}

void Player::Move() {
	if (input_->PressedKey(DIK_P)) {
		inverseVelSignal_ = true;
	}

	// 移動方向を反転する信号が送られた時の処理
	if (inverseVelSignal_) {
		// 方向ベクトル
		Vector3 velocity(0, 0, kSpeed);

		// 後ろを向かせる
		worldTransform_.rotate.y += (float)M_PI;
		// 2π以上なら0に初期化
		if (worldTransform_.rotate.y > (float)M_PI * 3.5f) {
			worldTransform_.rotate.y = (float)M_PI / 2;
		}
		// ワールド座標を更新
		worldTransform_.UpdateMatrix();

		// 向いている方向に進む。ただし,ワールド座標から見てy,z方向は反転させる必要がないので計算から除外
		velocity_.x = TransformNormal(velocity, worldTransform_.matWorld).x;

		// 信号をoff
		inverseVelSignal_ = false;
	}

	// 移動処理
	worldTransform_.translate = Add(worldTransform_.translate, velocity_);
}

#pragma region ふるまい

void Player::B_Init() {
	if (behaviorRequest_) {
		//  振るまいを変更
		behavior_ = behaviorRequest_.value();
		switch (behavior_) {
		case Behavior::kNormal:
		default:
			B_NormalInit();
			break;
			// ジャンプした瞬間
		case Behavior::kJump:
			B_JumpInit();
			break;
			// 空中
		case Behavior::kAir:
			B_AirInit();
			break;
			// 着地した瞬間
		case Behavior::kLanding:
			B_LandingInit();
			break;
		}
		// 振るまいリクエストをリセット
		behaviorRequest_ = std::nullopt;
	}
}
void Player::B_Update() {
	switch (behavior_) {
	case Behavior::kNormal:
	default:
		B_NormalUpdate();
		break;
		// ジャンプした瞬間
	case Behavior::kJump:
		B_JumpUpdate();
		break;
		// 空中
	case Behavior::kAir:
		B_AirUpdate();
		break;
		// 着地した瞬間
	case Behavior::kLanding:
		B_LandingUpdate();
		break;
	}
}

void Player::B_NormalInit() {
	velocity_.y = 0;
}
void Player::B_NormalUpdate() {
	if (isJump_) {
		behaviorRequest_ = Behavior::kJump;
	}
}

void Player::B_JumpInit() {
	// 速度を初期化
	velocity_.y = 0;
	// 初速を加算
	velocity_.y += initialVel_;
	isJump_ = false;
}
void Player::B_JumpUpdate() {
	behaviorRequest_ = Behavior::kAir;
}

void Player::B_AirInit() {
	hitCounter_ = 0;
}
void Player::B_AirUpdate() {
	if (worldTransform_.translate.y <= -5) {
		behaviorRequest_ = Behavior::kLanding;
	}
}

void Player::B_LandingInit() {

}
void Player::B_LandingUpdate() {
	behaviorRequest_ = Behavior::kNormal;
}

#pragma endregion

void Player::AdjustmentParameter() {
#ifdef _DEBUG
	ImGui::Begin("Player");

	if (ImGui::TreeNode("worldTransform")) {
		ImGui::DragFloat3("translate", &worldTransform_.translate.x, 0.1f, 100, 100);
		ImGui::DragFloat3("rotate", &worldTransform_.rotate.x, 0.01f, -6.28f, 6.28f);
		ImGui::DragFloat3("scale", &worldTransform_.scale.x, 0.01f, 0, 10);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Inverse Velocity")) {
		ImGui::DragFloat3("velocity", &velocity_.x, 0.01f, -1.0f, 1.0f);
		if (ImGui::Button("isActive")) {
			inverseVelSignal_ = true;
		}
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Behavior")) {
		if (ImGui::TreeNode("Normal")) {

			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Jump")) {
			ImGui::DragFloat("initial velocity", &initialVel_, 0.001f, 0, 50);
			ImGui::DragFloat3("acceleration", &acceleration_.x, 0.001f, -4.0f, 4.0f);
			ImGui::Checkbox("isActive", &isJump_);
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
	ImGui::Text("IsAlive:%d", isAlive_);

	ImGui::End();
#endif // DEBUG
}
