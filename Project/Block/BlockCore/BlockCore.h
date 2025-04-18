/// <summary>
/// ブロックのコア
/// </summary>
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


class BlockCore :public Collider {
public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	BlockCore();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~BlockCore();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(uint32_t texHandle_);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();


	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void UpdateIskLife();


	/// <summary>
	/// 描画
	/// </summary>
	void Draw(CameraRole viewProjection_);


	//衝突判定
	void OnCollision(Collider* collider)override;




	//ワールド座標系を取得
	Vector3 GetWorldPosition()override;


	/// Getter
	// ワールド座標を取得
	Vector3 GetworldTransform_() { return worldTransform_.translate; }
	// 生存フラグを取得
	bool GetIsAlive() { return isAlive_; }
	// 落ちているかを取得
	bool GetFoolFlag() { return foolflag_; }
	// 消えないブロックを取得
	inline bool GetIsHardBlock() { return isHardBlock_; }
	//スライドするかを取得
	bool GetIsSred() { return isSred_; }
	//デスフラグを取得
	bool GetIsDead()const { return isDead_; }
	//タイトルシーンかいなか
	bool GetIsTitleflag()const { return Titleflag_; }

	/// Setter
	// ワールド座標を設定
	void SetworldTransform_(Vector3 worldTransform) { this->worldTransform_.translate = worldTransform; }
	// 生存フラグを設定
	void SetIsAlive(bool isAlive) { isAlive_ = isAlive; }
	// 落ちているかを取得
	void SetFoolFlag(bool foolflag) { foolflag_ = foolflag; }
	// 消えないブロックを設定
	inline void SetIsHardBlock(bool isActive) { isHardBlock_ = isActive; }
	//ブロックの落ちるスピード
	void SetfoolSpeed(float foolSpeed) { foolSpeed_ = foolSpeed; }
	//スライドするかを設定
	void SetIsSred(bool isSred) { isSred_ = isSred; }
	//デスフラグの設定
	void SetIsDead(bool isDead) { isDead_ = isDead; }
	//タイトルシーンかいなか
	void SetIsTitleflag(bool Titleflag) { Titleflag_ = Titleflag; }
	//着地する場所
	void SetLandingPosition(float LandingPosition) { LandingPosition_ = LandingPosition; }

	//ワールド座標系を取得
	void SetWorldPosition(Vector3 translate);




private: // メンバ変数

	WorldTransform worldTransform_;
	//3Dモデル
	std::unique_ptr<Object3DPlacer> model_;

	//速度
	Vector3 velocity_;



	//寿命
	static const int32_t kLifeTime = 60 * 5;
	//デスタイマー
	int32_t deathTimer_ = kLifeTime;
	//デスフラグ
	bool isDead_ = false;

	//落ちるフラグ
	bool foolflag_ = false;
	// 生存フラグ
	bool isAlive_;

	//速度
	float foolSpeed_ = 0.4f;
	// 消えないブロック
	bool isHardBlock_;

	float radius = 4.0f;

	//ブロックの着地点
	float LandingPosition_ = -12;

	bool isSred_;

	bool Titleflag_ = false;
};