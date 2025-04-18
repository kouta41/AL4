#pragma once
/// <summary>
/// タイトルシーンのオブジェクト
/// </summary>
#include "Sprite.h"
#include "Object3D.h"
#include "ImGuiManager/ImGuiManager.h"
#include "Sprite/Sprite.h"
#include "BlockManager.h"
#include "GlobalVariables.h"
#include "Input/Input.h"
#include "BlockCore.h"


class TitleObject{
public://メンバ関数
	
	/// <summary>
	// コンストラクタ
	/// </summary>
	TitleObject();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TitleObject();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(CollisionManager* collisionManager);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(CameraRole camera);


	/// <summary>
	/// 描画
	/// </summary>
	void Draw2D(CameraRole camera);

	//調整項目の適任
	void ApplyGlobalVariaBles();

private://メンバ変数

	//当たり判定
	CollisionManager* collisionManager_;

	//調整項目
	GlobalVariables* globalVariables_;

	//ブロックマネージャー
	BlockManager* blockManager_;

	//落ちてくるブロック
	std::list<BlockCore*> cores_;

	//ブロックの着地点
	float LandingPosition_;

	//落ちてくるブロックの位置
	Vector3 randPos_;
	//落ちてくるタイミング
	float foolTime = 0;

	//落ちてくるタイミング
	float foolTime1 = 0;

	//キーボード入力
	Input* input_ = nullptr;

	
	//グループ名
	const char* groupName = "TitleObject";


	//テクスチャハンドル
	uint32_t coreTexHandle_ = 0;

};

