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

	coreTexHandle_ = TextureManager::Load("resources/cube.jpg");
	crustTexHandle_= TextureManager::Load("resources/uvChecker.png");


	select_ = std::make_unique<SelectionScenen>();

	// 7×7のプレイヤーのデータ
	

	/*ステージ１
	{0,0,0,0,0,0,0},
	{0,2,1,1,1,2,0},
	{0,2,2,2,2,2,0},
	{0,2,2,1,2,2,0},
	{0,2,2,2,2,2,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	*/

	/*ステージ２
	{0,0,0,0,0,0,0},
	{0,2,2,2,2,2,0},
	{0,2,1,2,1,2,0},
	{0,2,2,1,2,2,0},
	{0,2,1,2,1,2,0},
	{0,2,2,2,2,2,0},
	{0,0,0,0,0,0,0},	
	*/

	playerLocation_ =
	{
		{0,0,0,0,0,0,0},
		{0,0,0,1,0,0,0},
		{0,0,1,2,1,0,0},
		{0,0,2,2,2,0,0},
		{0,2,1,2,1,2,0},
		{0,2,2,0,2,2,0},
		{0,0,0,0,0,0,0},
	};


	if (select_->GetselectCount() == 1) {
		playerLocation_ =
		{
			{0,0,0,0,0,0,0},
			{0,2,1,0,0,0,0},
			{0,2,2,0,0,0,0},
			{0,2,2,0,0,0,0},
			{0,2,2,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
		};
	}

	if (select_->GetselectCount() == 2) {
		playerLocation_ =
		{
			{0,0,0,0,0,0,0},
			{0,2,1,1,0,0,0},
			{0,2,2,2,0,0,0},
			{0,2,2,1,0,0,0},
			{0,2,2,2,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
		};
	}

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

	texHandle_ = TextureManager::Load("resources/white.png");

	worldTransform_1.Initialize();
	worldTransform_2.Initialize();
	worldTransform_3.Initialize();
	worldTransform_4.Initialize();


	model_1 = std::make_unique<Object3DPlacer>();
	model_1->Initialize();
	model_1->SetModel("cube.obj");
	model_1->SetTexHandle(texHandle_);
	worldTransform_1.translate = { 10,5,0 };
	worldTransform_1.scale = { 1,1,1 };

	model_2 = std::make_unique<Object3DPlacer>();
	model_2->Initialize();
	model_2->SetModel("cube.obj");
	model_2->SetTexHandle(texHandle_);
	worldTransform_2.translate = { -10,-5,0 };
	worldTransform_2.scale = { 1,1,1 };


	model_3 = std::make_unique<Object3DPlacer>();
	model_3->Initialize();
	model_3->SetModel("cube.obj");
	model_3->SetTexHandle(texHandle_);
	worldTransform_3.translate = { 4,-10,0 };
	worldTransform_3.scale = { 1,1,1 };

	model_4 = std::make_unique<Object3DPlacer>();
	model_4->Initialize();
	model_4->SetModel("cube.obj");
	model_4->SetTexHandle(texHandle_);
	worldTransform_4.translate = { -4,10,0 };
	worldTransform_4.scale = { 1,1,1, };
	
}

void Player::Update(){
	worldTransform_.UpdateMatrix();

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

	worldTransform_1.UpdateMatrix();
	worldTransform_2.UpdateMatrix();
	worldTransform_3.UpdateMatrix();
	worldTransform_4.UpdateMatrix();

}



void Player::Draw(CameraRole viewProjection_){

	model_1->Draw(worldTransform_1, viewProjection_);
	model_2->Draw(worldTransform_2, viewProjection_);
	model_3->Draw(worldTransform_3, viewProjection_);
	model_4->Draw(worldTransform_4, viewProjection_);
	///デバック場面
#ifdef _DEBUG

	ImGui::Begin("WorldTransform");
	if (ImGui::TreeNode("worldTransform_1")) {
		ImGui::DragFloat3("translate", &worldTransform_1.translate.x, 0.1f, 100, 100);
		ImGui::DragFloat3("rotate", &worldTransform_1.rotate.x, 0.01f, -6.28f, 6.28f);
		ImGui::DragFloat3("scale", &worldTransform_1.scale.x, 0.01f, 0, 10);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("worldTransform_2")) {
		ImGui::DragFloat3("translate", &worldTransform_2.translate.x, 0.1f, 100, 100);
		ImGui::DragFloat3("rotate", &worldTransform_2.rotate.x, 0.01f, -6.28f, 6.28f);
		ImGui::DragFloat3("scale", &worldTransform_2.scale.x, 0.01f, 0, 10);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("worldTransform_3")) {
		ImGui::DragFloat3("translate", &worldTransform_3.translate.x, 0.1f, 100, 100);
		ImGui::DragFloat3("rotate", &worldTransform_3.rotate.x, 0.01f, -6.28f, 6.28f);
		ImGui::DragFloat3("scale", &worldTransform_3.scale.x, 0.01f, 0, 10);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("worldTransform_4")) {
		ImGui::DragFloat3("translate", &worldTransform_4.translate.x, 0.1f, 100, 100);
		ImGui::DragFloat3("rotate", &worldTransform_4.rotate.x, 0.01f, 100, 100);
		ImGui::DragFloat3("scale", &worldTransform_4.scale.x, 0.01f, 100, 100);
		ImGui::TreePop();
	}


	ImGui::End();

#endif // _DEBUG
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
	Vector3 worldPos{};

	for (PlayerCore* core_ : cores_) {
		worldPos = core_->GetWorldPosition();
	}
	return worldPos;
}

