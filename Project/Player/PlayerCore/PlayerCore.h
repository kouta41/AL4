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


class PlayerCore :public Collider {
public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	PlayerCore();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PlayerCore();

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

	//移動制限
	void MovementRestrictions();


	//ワールド座標系を取得
	Vector3 GetWorldPosition();

	//ワールド座標系を取得
	void SetWorldPosition(Vector3 translate);

	bool IsDead()const { return isDead_; }

	bool isWstop()const { return isWstop_; }
	bool isAstop()const { return isAstop_; }
	bool isSstop()const { return isSstop_; }
	bool isDstop()const { return isDstop_; }



private: // メンバ変数

	WorldTransform worldTransform_;
	//3Dモデル
	std::unique_ptr<Object3DPlacer> model_;

	//速度
	Vector3 velocity_;



	//寿命
	static const int32_t kLifeTime = 60 * 3;
	//デスタイマー
	int32_t deathTimer_ = kLifeTime;
	//デスフラグ
	bool isDead_ = false;

	//移動制限フラグ
	bool isWstop_ = false;
	bool isAstop_ = false;
	bool isSstop_ = false;
	bool isDstop_ = false;

	//速度
	float foolSpeed_ = 0.4f;
};