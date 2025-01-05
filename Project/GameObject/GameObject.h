/// <summary>
/// ゲームオブジェクト
/// </summary>
#pragma once
#include "Sprite.h"
#include "ImGuiManager/ImGuiManager.h"
#include <Object3D.h>
#include "Sprite/Sprite.h"
#include "BlockManager.h"

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


	/// <summary>
	/// 描画
	/// </summary>
	void Draw2D(CameraRole viewProjection_);


	///geter
	void SetPlayer(BlockManager* blockManager) { blockManager_ = blockManager; }



private://メンバ変数

	BlockManager* blockManager_;

	//ワールド座標
	WorldTransform worldTransform_Right;
	WorldTransform worldTransform_Left;
	WorldTransform worldTransform_Bottom;

	WorldTransform spriteWorldTransform;
	WorldTransform spriteWorldTransform_chage;
	WorldTransform spriteWorldTransform_ON;
	WorldTransform spriteWorldTransform_OFF;


	//3Dモデル
	std::unique_ptr<Object3DPlacer> Stagemodel_Right;
	std::unique_ptr<Object3DPlacer> Stagemodel_Left;
	std::unique_ptr<Object3DPlacer> Stagemodel_Bottom;

	//2Dスプライト
	std::unique_ptr<Sprite> UIsprite_;
	std::unique_ptr<Sprite> UIsprite_chage;
	std::unique_ptr<Sprite> UIsprite_ON;
	std::unique_ptr<Sprite> UIsprite_OFF;


	//テクスチャハンドル
	uint32_t texHandle_ = 0;
	uint32_t texHandle_UI = 0;
	uint32_t texHandle_chage = 0;
	uint32_t texHandle_ON = 0;
	uint32_t texHandle_OFF = 0;


};

