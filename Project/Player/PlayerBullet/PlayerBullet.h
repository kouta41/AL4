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


class PlayerBullet :public Collider {
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
	void Draw(CameraRole viewProjection_);


	//衝突判定
	void OnCollision();


	bool IsDead()const { return isDead_; }
	//ワールド座標系を取得
	Vector3 GetWorldPosition();
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
};
