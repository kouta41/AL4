#include "RailCamera.h"

void RailCamera::Initialize(Vector3 pos, Vector3 radius){
	worldTransform_.translate = pos;
	worldTransform_.rotate = radius;
	//ビュープロジェクションの初期化
	viewProjection_.farZ = 10000;
	viewProjection_.Initialize();
	worldTransform_.Initialize();
}

void RailCamera::Update(){
	viewProjection_.UpdateMatrix();
	worldTransform_.UpdateMatrix();
	//移動（ベクトルを加算）
	worldTransform_.translate.z += 0.05f; 
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
