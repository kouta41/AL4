#include "Player.h"
#include <Skybox.h>

Player::Player() {

}

Player::~Player() {

}

void Player::Initialize(CollisionManager* collisionManager_) {
	worldTransform_.Initialize();
}


void Player::Update() {

}

void Player::Draw(const CameraRole& camera_) {

}

void Player::OnCollision(Collider* collider) {

}


Vector3 Player::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld.m[3][0];
	worldPos.y = worldTransform_.matWorld.m[3][1];
	worldPos.z = worldTransform_.matWorld.m[3][2];

	return worldPos;
}
