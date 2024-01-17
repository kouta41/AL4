#pragma once
#include"Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include "TextureManager.h"
#include <cassert>

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

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	std::unique_ptr<Model> model_{};
	//テクスチャハンドル
	uint32_t texHandle_ = 0u;

	//速度
	Vector3 velocity_;

};
