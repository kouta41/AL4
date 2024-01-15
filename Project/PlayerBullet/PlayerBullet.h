#pragma once
#include "Sprite.h"
#include "Triangle.h"
#include "Model.h"
#include "ModelSphere.h"
#include "ModelCube.h"
#include "Input.h"
#include "ImGuiManager/ImGuiManager.h"


class PlayerBullet{
public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	PlayerBullet();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PlayerBullet();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(uint32_t texHandle_,const Vector3& position, const Vector3& velocity);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection viewProjection_);

	bool IsDead()const { return isDead_; }

private: // メンバ変数

	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
	//3Dモデル
	std::unique_ptr<Model> model_{};

	//速度
	Vector3 velocity_;

	//テクスチャハンドル
	uint32_t texHandle_ = 0;

	//寿命
	static const int32_t kLifeTime = 60 * 3;
	//デスタイマー
	int32_t deathTimer_ = kLifeTime;
	//デスフラグ
	bool isDead_ = false;
};

