#pragma once
#include "Sprite.h"
#include "ImGuiManager/ImGuiManager.h"
#include <Object3D.h>

class GameObject{

public://メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameObject();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameObject();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(CameraRole viewProjection_);

private://メンバ変数

	//ワールド座標
	WorldTransform worldTransform_Right;
	WorldTransform worldTransform_Left;
	WorldTransform worldTransform_Bottom;

	//3Dモデル
	std::unique_ptr<Object3DPlacer> Stagemodel_Right;
	std::unique_ptr<Object3DPlacer> Stagemodel_Left;
	std::unique_ptr<Object3DPlacer> Stagemodel_Bottom;

	//テクスチャハンドル
	uint32_t texHandle_ = 0;
};

