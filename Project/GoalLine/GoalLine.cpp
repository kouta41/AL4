#include "GoalLine.h"

void GoalLine::Initialize() {
	// 使用するテクスチャを読み込む
	lineTexture_ = TextureManager::Load("Resources/yellow.jpg");
	ModelManager::LoadObjModel("block.obj");


	// 3Dモデル
	model_ = std::make_unique<Object3DPlacer>();
	model_->Initialize();
	model_->SetModel("block.obj");
	model_->SetTexHandle(lineTexture_);

	worldTransform_.Initialize();
	worldTransform_.translate = { 0,9,-2.5f };
	worldTransform_.scale = { 100,0.1f,1 };
	worldTransform_.UpdateMatrix();

	// ゴールフラグ
	isGoal_ = false;
}

void GoalLine::Update() {
	// ゴールラインより自機が上に行ったらクリア
	if (player_->GetWorldPosition().y >= worldTransform_.translate.y) {
		isGoal_ = true;
	}

	worldTransform_.UpdateMatrix();
#ifdef _DEBUG

	ImGui::Begin("GoalLine");
	if (ImGui::TreeNode("worldTransform")) {
		ImGui::DragFloat3("translate", &worldTransform_.translate.x, 0.1f, 100, 100);
		ImGui::DragFloat3("rotate", &worldTransform_.rotate.x, 0.01f, -6.28f, 6.28f);
		ImGui::DragFloat3("scale", &worldTransform_.scale.x, 0.01f, 0, 10);
		ImGui::TreePop();
	}
	ImGui::End();
#endif // RELEASE

}

void GoalLine::Draw3DLine(const CameraRole& viewProjection) {
	// ワールド座標上の3Dライン
	model_->Draw(worldTransform_, viewProjection);

}


