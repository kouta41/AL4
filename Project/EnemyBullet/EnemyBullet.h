#pragma once
#include"Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include <cassert>

#include "Object3D.h"
#include "Player.h"

class Player;
class PlayerCore;

class EnemyBullet :public Collider {

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(uint32_t texHandle_, const Vector3& position, const Vector3& velocity);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const CameraRole& viewProjection_);

	//衝突判定
	void OnCollision();

	bool IsDead()const { return isDead_; }

	//自機座標
	void SetPlayer(Player* player) { player_ = player; }

	void SetPlayerCorepos(Vector3 cores) { coresPos_ = cores; }

	//ワールド座標系を取得
	Vector3 GetWorldPosition();
private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	std::unique_ptr<Object3DPlacer> model_;

	//テクスチャハンドル
	uint32_t texHandle_ = 0u;

	//速度
	Vector3 velocity_;
	Vector3 toPlayer;


	//自機
	Player* player_;

	///核
	Vector3 coresPos_;
	std::list<PlayerCore*> cores_;
	
	//寿命
	static const int32_t kLifeTime = 300;
	//デスタイマー
	int32_t deathTimer_ = kLifeTime;
	//デスフラグ
	bool isDead_ = false;
};
