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

	// 3Dレティクルのワールドトランスフォーム初期化
	worldTransform3DReticle_.Initialize();
	// レティクル用テクスチャ取得
	textureReticle_ = TextureManager::Load("resources/tama.png");
	// スプライト生成
	sprite2DReticle_.reset(Sprite::Create({ 640.0f,360.0f }, { 150.0f,150.0f }, { 0.5f,0.5f }));
	worldTransform3DReticle_.Initialize();
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
		worldTransform_.translate.x += 0.6f;
	}
	else if (input_->PushKey(DIK_LEFT)) {
		worldTransform_.translate.x -= 0.6f;
	}
	if (input_->PushKey(DIK_UP)) {
		worldTransform_.translate.y += 0.6f;
	}
	else if (input_->PushKey(DIK_DOWN)) {
		worldTransform_.translate.y -= 0.6f;
	}

	/*if (input_->PushKey(DIK_W)) {
		worldTransform_.translate.z += 0.1f;
	}
	else if (input_->PushKey(DIK_S)) {
		worldTransform_.translate.z -= 0.1f;
	}*/

	/*if (input_->PushKey(DIK_A)) {
		worldTransform_.rotate.y -= 0.02f;
	}
	else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotate.y += 0.02f;
	}*/

	Attack();
	for(PlayerBullet* bullet_:bullets_){
		bullet_->Update();
	}
	Vector2 spritePosition = sprite2DReticle_->GetPosition();


	if (input_->PushKey(DIK_A)) {
		spritePosition.x -= 8.0f;
	}
	else if (input_->PushKey(DIK_D)) {
		spritePosition.x += 8.0f;
	}
	
	if (input_->PushKey(DIK_W)) {
		spritePosition.y -= 8.0f;
	}
	else if (input_->PushKey(DIK_S)) {
		spritePosition.y += 8.0f;
	}
	// レティクル
	Reticle(viewProjection_, Vector2((float)spritePosition.x, (float)spritePosition.y));

}

void Player::Attack(){
	if (input_->PushKeyPressed(DIK_SPACE)) {
		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity = { 0,0,kBulletSpeed };
		// 自機から照準オブジェクトのベクトル
		Vector3 WorldPos = GetWorldPosition();
		Vector3 ReticleWorldPos = GetWorldPosition3DReticle();
		velocity = Subtract(ReticleWorldPos, WorldPos);
		velocity = Normalize(velocity);
		velocity = Multiply(kBulletSpeed, velocity);
		// プレイヤーの向きに速度を合わせる
		velocity = TransformNormal(velocity, worldTransform_.matWorld);
		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(texHandleBullet_, worldTransform_.translate, velocity);
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

void Player::DrawUI(ViewProjection viewProjection_)
{
	sprite2DReticle_->Draw(viewProjection_, textureReticle_);
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

Vector3 Player::GetWorldPosition3DReticle()
{
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform3DReticle_.matWorld.m[3][0];
	worldPos.y = worldTransform3DReticle_.matWorld.m[3][1];
	worldPos.z = worldTransform3DReticle_.matWorld.m[3][2];

	return worldPos;
}

void Player::setParent(const WorldTransform* parent){
	//親子関係を結ぶ
	worldTransform_.parent = parent;
}

void Player::Reticle(const ViewProjection& viewProjection_, const Vector2& pos)
{
	// スプライトのレティクルに座標設定
	sprite2DReticle_->SetPosition(pos);

	// ビューポート行列
	Matrix4x4 matViewport =
		MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

	// ビュープロジェクションビューポート合成行列
	Matrix4x4 matVPN =
		Multiply(viewProjection_.matView, Multiply(viewProjection_.matProjection, matViewport));
	// 逆行列を計算
	Matrix4x4 matInverseVPN = Inverse(matVPN);

	// スクリーン座標
	Vector3 posNear = Vector3((float)pos.x, (float)pos.y, 1);
	Vector3 posFar = Vector3((float)pos.x, (float)pos.y, 0);

	// スクリーン座標からワールド座標系へ
	posNear = Transform(posNear, matInverseVPN);
	posFar = Transform(posFar, matInverseVPN);

	// マウスレイの方向
	Vector3 mouseDirection = Subtract(posNear, posFar);
	mouseDirection = Normalize(mouseDirection);
	// カメラから照準オブジェクトの距離
	const float kDistanceTestObject = 100.0f;
	worldTransform3DReticle_.translate = Multiply(kDistanceTestObject, mouseDirection);
	worldTransform3DReticle_.UpdateMatrix();
}
