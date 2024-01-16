#pragma once
#include "Sprite.h"
#include "Triangle.h"
#include "Model.h"
#include "ModelSphere.h"
#include "ModelCube.h"
#include "Input.h"
#include "ImGuiManager/ImGuiManager.h"
#include "PlayerBullet.h"

#include <cassert>

enum class Phase {
	Approach,	//接近
	Leave,		//離脱
};

class Enemy;

class BaseEnemyState {
public:
	virtual void Update(Enemy* pEnemy) = 0;
};

class EnemyStateApproach :public BaseEnemyState {
public:
	void Update(Enemy* pEnemy);
};

class EnemyStateLeave :public BaseEnemyState {
public:
	void Update(Enemy* pEnemy);
};


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

	void Move();

	void ChangeState(BaseEnemyState* newState);

	void SetVelocity(float x, float y, float z);

	Vector3 GetWorldTransform() { return worldTransform_.translate; }
	Vector3 GetVelocity() { return velocity_; }
private: // メンバ変数

	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

	//3Dモデル
	std::unique_ptr<Model> model_{};


	//キーボード入力
	Input* input_ = nullptr;

	//弾
	//std::list<PlayerBullet*> bullets_;

	//移動速度
	Vector3 velocity_;

	//フェーズ
	Phase phase_ = Phase::Approach;
	 
	//テクスチャハンドル
	uint32_t texHandle_ = 0;
	uint32_t texHandleBullet_ = 0;

	//ステート
	BaseEnemyState* state;
};

