#include "RailCamera.h"

void RailCamera::Initialize(const Vector3& position, const Vector3& rotate){
	// ワールドトランスフォーム
	worldTransform_.translate = position;
	worldTransform_.rotate = rotate;
	worldTransform_.scale = { 1.0f, 1.0f, 1.0f };
	//ビュープロジェクションの初期化
	viewProjection_.farZ = 20000;
	viewProjection_.Initialize();
	controlPoints_ = {
		{0,  0,  0 },
		{10, 10, 0 },
		{10, 15, 0 },
		{20, 15, 0 },
		{20, 0,  0 },
		{30, 0,  0 },
	};
}

void RailCamera::Update(){
	
	//移動（ベクトルを加算）
//	worldTransform_.translate.z += 0.05f; 
	worldTransform_.matWorld = MakeAffineMatrix(worldTransform_.scale, worldTransform_.rotate, worldTransform_.translate);

	//カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection_.matView = Inverse(worldTransform_.matWorld);

	ImGui::Begin("Camera");
	if (ImGui::TreeNode("worldTransform")) {
		ImGui::DragFloat3("translate", &worldTransform_.translate.x, 0.1f, 100, 100);
		ImGui::DragFloat3("rotate", &worldTransform_.rotate.x, 0.01f, -6.28f, 6.28f);
		ImGui::DragFloat3("scale", &worldTransform_.scale.x, 0.01f, 0, 10);
		ImGui::TreePop();
	}
	ImGui::End();
}
