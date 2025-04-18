#include "BlockCore.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <cassert>

BlockCore::BlockCore(){
}

BlockCore::~BlockCore(){
}

void BlockCore::Initialize(uint32_t texHandle_) {
	worldTransform_.Initialize();

	model_ = std::make_unique<Object3DPlacer>();
	model_->Initialize();
	model_->SetModel("cube.obj");
	model_->SetTexHandle(texHandle_);



	//落ちているか
	foolflag_ = true;

	// 生存フラグ
	isAlive_ = true;

	//スライドするか
	isSred_ = true;

	// 当たり判定の形状を設定
	SetCollisionPrimitive_(kCollisionAABB);
	//当たり判定の属性
	SetCollisionAttribute_(kAttributeBlock);

	AABB aabb = {
		{-0.99999f,-1.0f,-0.99999f},
		{0.99999f,1.0f,0.99999f}
	};
	//SetAABB(aabb);
}

void BlockCore::Update(){
	worldTransform_.UpdateMatrix();
		
	worldTransform_.translate.y -= foolSpeed_;

	if (Titleflag_ == false) {
		if (worldTransform_.translate.y <= LandingPosition_) {
			float floor = worldTransform_.translate.y - (LandingPosition_);
			worldTransform_.translate.y -= floor;
			foolflag_ = false;
			SetIsBottomHitAABB_(true);
		}
	}
	else {
		//時間経過でデス
		if (--deathTimer_ <= 0) {
			isDead_ = true;
		}
	}
}

void BlockCore::UpdateIskLife(){
	//時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

}

void BlockCore::Draw(CameraRole viewProjection_){
	model_->Draw(worldTransform_, viewProjection_);
}

void BlockCore::OnCollision(Collider* collider){
	float theta = atan2(worldTransform_.translate.y - collider->GetWorldPosition().y, worldTransform_.translate.x - collider->GetWorldPosition().x);

	if (GetCollisionAttribute_() == collider->GetCollisionAttribute_()) {
		// 下
		if (theta >= (M_PI / radius) && theta <= M_PI - (M_PI / radius)) {
			float extrusion = (-GetAABB_().min.y + collider->GetAABB_().max.y) - (worldTransform_.translate.y - collider->GetWorldPosition().y);
			worldTransform_.translate.y += extrusion;
			int y = static_cast<int>(std::round(worldTransform_.translate.y));
			worldTransform_.translate.y = (float)y;
			//worldTransform_.UpdateMatrix();
			foolflag_ = false;
			SetIsBottomHitAABB_(true);
		}
		else {
			SetIsBottomHitAABB_(false);
			//foolflag_ = true;
		}

		// 上
		if (theta <= -(M_PI / radius) && theta >= -M_PI + (M_PI / radius)) {
			//worldTransform_.UpdateMatrix();
			if (GetCollisionAttribute_() == collider->GetCollisionAttribute_()) {
				SetIsTopHitAABB_(true);
			}
		}
		else {
			SetIsTopHitAABB_(false);
		}

		// 右
		if (theta < M_PI / radius && theta > -(M_PI / radius)) {
			float extrusion = (-GetAABB_().min.x + collider->GetAABB_().max.x) - (worldTransform_.translate.x - collider->GetWorldPosition().x);
			worldTransform_.translate.x += extrusion;
		//	worldTransform_.UpdateMatrix();
			if (GetCollisionAttribute_() == collider->GetCollisionAttribute_()) {
				SetIsRightHitAABB_(true);
			}
		}
		// 左
		if (theta > M_PI - (M_PI / radius) || theta < -M_PI + (M_PI / radius)) {
			float extrusion = (GetAABB_().max.x + (-collider->GetAABB_().min.x)) - (collider->GetWorldPosition().x - worldTransform_.translate.x);
			worldTransform_.translate.x -= extrusion;
		//	worldTransform_.UpdateMatrix();
			if (GetCollisionAttribute_() == collider->GetCollisionAttribute_()) {
				SetIsLeftHitAABB_(true);
			}
		}
	}
}



Vector3 BlockCore::GetWorldPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld.m[3][0];
	worldPos.y = worldTransform_.matWorld.m[3][1];
	worldPos.z = worldTransform_.matWorld.m[3][2];

	return worldPos;
}

void BlockCore::SetWorldPosition(Vector3 translate){
	worldTransform_.translate.x = translate.x;
	worldTransform_.translate.y = translate.y;
	worldTransform_.translate.z = translate.z;
}
