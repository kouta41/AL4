/// <summary>
/// ゲームシーンのオブジェクト
/// </summary>
#pragma once
#include "Sprite.h"
#include "Object3D.h"
#include "ImGuiManager/ImGuiManager.h"
#include "Sprite/Sprite.h"
#include "BlockManager.h"
#include "GlobalVariables.h"
#include "Input/Input.h"

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

	//オプションの有無
	bool GetoptionFlag() { return optionFlag_; }

	//setter
	void SetoptionFlag(bool optionFlag) { optionFlag_ = optionFlag; }

	//調整項目の適任
	void ApplyGlobalVariaBles();

private://メンバ変数


	//調整項目
	GlobalVariables* globalVariables_;

	//ブロックマネージャー
	BlockManager* blockManager_;

	//ワールド座標
	WorldTransform worldTransform_Right;
	WorldTransform worldTransform_Left;
	WorldTransform worldTransform_Bottom;

	WorldTransform spriteWorldTransform;
	WorldTransform spriteWorldTransform_chage;
	WorldTransform spriteWorldTransform_ON;
	WorldTransform spriteWorldTransform_OFF;
	WorldTransform spriteWorldTransform_option;
	WorldTransform spriteworldTransform_crossKeyGuide;
	WorldTransform spriteworldTransform_Space;
	WorldTransform spriteworldTransform_SpaceGuide;
	WorldTransform spriteworldTransform_GuideUI;
	WorldTransform spriteworldTransform_FallGuide;
	WorldTransform spriteworldTransform_DeleteString;
	WorldTransform spriteworldTransform_DeleteStringUI;


	WorldTransform nextWorldTransform_[5];
	WorldTransform secondWorldTransform_[5];

	WorldTransform worldTransform_background;
	WorldTransform worldTransform_background1;

	WorldTransform worldTransform_guide;


	//キーボード入力
	Input* input_ = nullptr;

	//3Dモデル
	std::unique_ptr<Object3DPlacer> Stagemodel_Right;
	std::unique_ptr<Object3DPlacer> Stagemodel_Left;
	std::unique_ptr<Object3DPlacer> Stagemodel_Bottom;

	std::unique_ptr<Object3DPlacer> nextModel_[4];
	std::unique_ptr<Object3DPlacer> scandModel_[4];

	std::unique_ptr<Object3DPlacer> backgroundModel_;

	std::unique_ptr<Object3DPlacer> guideModel_;

	std::unique_ptr<Object3DPlacer> backgroundModel_1;


	//2Dスプライト
	std::unique_ptr<Sprite> UIsprite_;
	std::unique_ptr<Sprite> UIsprite_chage;
	std::unique_ptr<Sprite> UIsprite_ON;
	std::unique_ptr<Sprite> UIsprite_OFF;
	std::unique_ptr<Sprite> UIsprite_option;
	std::unique_ptr<Sprite> Operation_crossKeyGuide;
	std::unique_ptr<Sprite> Operation_SpaceGuide;
	std::unique_ptr<Sprite> Operation_Space;
	std::unique_ptr<Sprite> Operation_GuideUI;
	std::unique_ptr<Sprite> Operation_FallGuide;
	std::unique_ptr<Sprite> Operation_DeleteString;
	std::unique_ptr<Sprite> Operation_DeleteStringUI;

	//テクスチャハンドル
	uint32_t texHandle_ = 0;
	uint32_t texHandle_UI = 0;
	uint32_t texHandle_chage = 0;
	uint32_t texHandle_ON = 0;
	uint32_t texHandle_OFF = 0;
	uint32_t texHandle_Block = 0;
	uint32_t texHandle_background = 0;
	uint32_t texHandle_background1 = 0;
	uint32_t texHandle_guide = 0;
	uint32_t texHandle_moji = 0;
	uint32_t texHandle_crossKeyGuide = 0;
	uint32_t texHandle_SpaceGuide = 0;
	uint32_t texHandle_Space = 0;
	uint32_t texHandle_GuideUI = 0;
	uint32_t texHandle_FallGuide = 0;
	uint32_t texHandle_DeleteString = 0;
	uint32_t texHandle_DeleteStringUI = 0;


	//グループ名
	const char* groupName = "GameObject";

	//
	Shape nextShape[3];

	//ブロックガイドのオフセット
	Vector3 BlockGuide_Offset;

	//次のブロックの座標(X軸の三種){X<Y<Z}
	Vector3 NextTranslate_X;
	//次のブロックの座標(Y軸の四種){X<Y<Z<W}
	Vector4 NextTranslate_Y;

	//次の次のブロックの座標(X軸の三種){X<Y<Z}
	Vector3 secondTranslate_X;
	//次の次のブロックの座標(Y軸の四種){X<Y<Z<W}
	Vector4 secondTranslate_Y;

	//他のブロックの角度調整
	Vector3 OtherBlock_rotate;

	//オプションフラグ
	bool optionFlag_ = false;
};

