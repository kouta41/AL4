#pragma once
#include "Sprite.h"
#include "Triangle.h"
#include "Model.h"
#include "ModelSphere.h"
#include "ModelCube.h"
#include "ModelManager.h"
#include "Input.h"
#include "ImGuiManager/ImGuiManager.h"
#include "EnemyBullet.h"
#include "TimedCall.h"
#include <cassert>
#include "Collider.h"
#include "Object3D.h"

enum class Phase {
	Approach,	//接近
	Leave,		//離脱
};

class Player;
class GameScene;
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


class Enemy:public Collider {

	
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
	/// 攻撃
	/// </summary>
	void Fire();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(CameraRole viewProjection_);

	void Move();

	//衝突判定
	void OnCollision();

	//弾リストを取得
	const std::list<EnemyBullet*>& GetEnemyBullsts()const { return bullets_; }

	//ワールド座標系を取得
	Vector3 GetWorldPosition();

	void ChangeState(BaseEnemyState* newState);

	void SetVelocity(float x, float y, float z);

	void SetPlayer(Player* player) { player_ = player; }

	//フェーズごとのアップデート
	void ApproachUpdate();
	//フェーズごとの初期化
	void ApproachInitialize();
	void LeaveInitialize();

	Vector3 GetWorldTransform() { return worldTransform_.translate; }
	Vector3 GetVelocity() { return velocity_; }

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

private: // メンバ変数

	WorldTransform worldTransform_;
	CameraRole viewProjection_;

	//3Dモデル
	std::unique_ptr<Object3DPlacer> model_;


	//キーボード入力
	Input* input_ = nullptr;

	//弾
	std::list<EnemyBullet*> bullets_;

	//移動速度
	Vector3 velocity_;

	//フェーズ
	Phase phase_ = Phase::Approach;
	 
	//テクスチャハンドル
	uint32_t texHandle_ = 0;
	uint32_t texHandleBullet_ = 0;

	//ステート
	BaseEnemyState* state_;

	std::list<TimedCall*> timedCalls_;

	//発射間隔
	int kFireInterval = 60;

	//自機
	Player* player_;

	//ゲームシーン
	GameScene* gameScene_ = nullptr;

	//デスフラグ
	bool isDead_ = false;
};

