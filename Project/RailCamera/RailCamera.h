#pragma once
#include "WorldTransform.h"
#include "CameraRole.h"
#include "MathFunction.h"
#include "ImGuiManager.h"
#include <imgui.h>
#include <vector>

class RailCamera {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const Vector3& position, const Vector3& rotate);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();


	const ViewProjection& GetViewProjection_() { return viewProjection_; }
	const WorldTransform& GetworldTransform_() { return worldTransform_; }
private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//ビュープロジェクション
	ViewProjection 	viewProjection_;

	//速度
	Vector3 velocity_ = { 0.0,0.0,0.0 };

	//レールカメラの移動曲線
	//スプライン曲線制御点(通過点)
	std::vector<Vector3> controlPoints_;
};