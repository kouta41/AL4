#pragma once
#include "WorldTransform.h"
#include "CameraRole.h"
#include "MathFunction.h"
#include "ImGuiManager.h"

class RailCamera {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Vector3 pos, Vector3 radius);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	ViewProjection& GetViewProjection_() { return viewProjection_; }
	WorldTransform& GetworldTransform_() { return worldTransform_; }
private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//ビュープロジェクション
	ViewProjection 	viewProjection_;

	//速度
	Vector3 velocity_ = { 0.0,0.0,0.0 };


};