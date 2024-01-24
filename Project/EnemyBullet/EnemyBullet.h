#pragma once
#include"Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include "TextureManager.h"
#include <cassert>

#include "Player.h"
class Player;

class EnemyBullet {

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
	void Draw(const ViewProjection& viewProjection_);
	bool IsDead()const { return isDead_; }

	//自機座標
	void SetPlayer(Player* player) { player_ = player; }

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	std::unique_ptr<Model> model_{};
	//テクスチャハンドル
	uint32_t texHandle_ = 0u;

	//速度
	Vector3 velocity_;
	Vector3 toPlayer;
	Player* player_;

	//寿命
	static const int32_t kLifeTime = 120;
	//デスタイマー
	int32_t deathTimer_ = kLifeTime;
	//デスフラグ
	bool isDead_ = false;
};
