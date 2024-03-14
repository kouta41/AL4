#include "RailCamera.h"

void RailCamera::Initialize(const Vector3& position, const Vector3& rotate){
	// ワールドトランスフォーム
	worldTransform_.translate = position;
	worldTransform_.rotate = rotate;
	//ビュープロジェクションの初期化
	viewProjection_.farZ = 10000;
	viewProjection_.Initialize();

}

void RailCamera::Update(){
	
	//移動（ベクトルを加算）
	worldTransform_.matWorld = MakeAffineMatrix(worldTransform_.scale, worldTransform_.rotate, worldTransform_.translate);

	//カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection_.matView = Inverse(worldTransform_.matWorld);


	ImGui::Begin("Camera");
	ImGui::SliderFloat3("pos", &worldTransform_.translate.x, -20.0f, 20.0f);
	ImGui::SliderFloat3("rsdius", &worldTransform_.rotate.x, -20.0f, 20.0f);

	ImGui::End();
}
