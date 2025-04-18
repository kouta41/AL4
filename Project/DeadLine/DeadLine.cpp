#include "DeadLine.h"

void DeadLine::Initialize() {
	// 使用するテクスチャを読み込む
	lineTexture_ = TextureManager::Load("Resources/surotto.png");
	

	// 3Dモデル
	model_ = std::make_unique<Object3DPlacer>();
	model_->Initialize();
	model_->SetModel("block.obj");
	model_->SetTexHandle(lineTexture_);

	// 3Dモデルの初期設定
	worldTransform_.Initialize();
	worldTransform_.translate = { 0,kStartDeadLine_,-1 };
	worldTransform_.scale = { 100,0.1f,1 };
	worldTransform_.UpdateMatrix();

	isBlockDelete_ = false;

	isDead_ = false;
}

void DeadLine::Update() {
	// 敗北条件
	if (player_->GetWorldPosition().y <= worldTransform_.translate.y) {
		isDead_ = true;
	}

	// 上昇処理
	worldTransform_.translate.y += kSpeed_;

	// ブロックが消されたらデッドラインを下げる
	if (isBlockDelete_ == true) {
		worldTransform_.translate.y -= kDownValue;
	}
	 
	worldTransform_.UpdateMatrix();
#ifdef _DEBUG

	ImGui::Begin("GoalLine");
	if (ImGui::TreeNode("worldTransform")) {
		ImGui::DragFloat3("translate", &worldTransform_.translate.x, 0.1f, 100, 100);
		ImGui::TreePop();
	}
	ImGui::End();
#endif // RELEASE
}

void DeadLine::Draw3DLine(const CameraRole& viewProjection) {
	// ワールド座標上の3Dライン
	model_->Draw(worldTransform_, viewProjection);
}
