#pragma once
#include "Sprite.h"
#include "Triangle.h"
#include "Model.h"
#include "ModelSphere.h"
#include "ModelCube.h"
#include "ModelManager.h"
#include "Input.h"
#include "ImGuiManager/ImGuiManager.h"
#include "Collider.h"
#include "Object3D.h"


class PlayerCrust :public Collider {
public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	PlayerCrust();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PlayerCrust();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(uint32_t texHandle_);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update(Vector3 velocity);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(CameraRole viewProjection_);


	//衝突判定
	void OnCollision();


	bool IsDead()const { return isDead_; }


	//ワールド座標系を取得
	Vector3 GetWorldPosition();
	//ワールド座標系を取得
	void SetWorldPosition(Vector3 translate);
private: // メンバ変数

	WorldTransform worldTransform_;
	CameraRole viewProjection_;
	//3Dモデル
	std::unique_ptr<Object3DPlacer> model_;

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

