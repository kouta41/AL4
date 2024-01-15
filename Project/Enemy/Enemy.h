#pragma once
#include "Sprite.h"
#include "Triangle.h"
#include "Model.h"
#include "ModelSphere.h"
#include "ModelCube.h"
#include "Input.h"
#include "ImGuiManager/ImGuiManager.h"
#include "PlayerBullet.h"

class Enemy{

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	Enemy();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Enemy();

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
	void Draw(ViewProjection viewProjection_);

private: // メンバ変数

	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

	//3Dモデル
	std::unique_ptr<Model> model_{};


	//キーボード入力
	Input* input_ = nullptr;

	//弾
	//std::list<PlayerBullet*> bullets_;

	//テクスチャハンドル
	uint32_t texHandle_ = 0;
	uint32_t texHandleBullet_ = 0;
};

