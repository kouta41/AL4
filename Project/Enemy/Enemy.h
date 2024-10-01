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
#include "CollisionManager.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


class Player;
class GameScene;
class Enemy;
class PlayerCore;

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
	///	真下に落ちるタイプ
	/// </summary>
	void UnderFall();

	/// <summary>
	/// 地面に向かって落ちるタイプ
	/// </summary>
	void TargetFall();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(CameraRole viewProjection_);


	//衝突判定
	void OnCollision();

	//当たり判定
	void CheckAllCollisions();

	//弾リストを取得
	const std::list<EnemyBullet*>& GetEnemyBullsts()const { return bullets_; }

	//ワールド座標系を取得
	Vector3 GetWorldPosition();


	void SetVelocity(float x, float y, float z);


	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	void SetPlayer(Player* player) { player_ = player; }

	void SetPlayerCorepos(Vector3 cores) { coresPos_ = cores; }

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
	Vector3 velocity_ = { 1,1,1 };
	 
	//テクスチャハンドル
	uint32_t texHandle_ = 0;
	uint32_t texHandleBullet_ = 0;


	std::list<TimedCall*> timedCalls_;

	//発射間隔
	Vector3 kFireInterval = { 0,0,0 };

	//自機
	Player* player_;

	///核
	Vector3 coresPos_;
	std::list<PlayerCore*> cores_;

	//目標対象
	Vector3 TargetPos_ = { 0,0,0 };

	//ゲームシーン
	GameScene* gameScene_ = nullptr;

	//デスフラグ
	bool isDead_ = false;


	//ポップフラグ
	int pop = 0;
	bool isFire_ = false;
	int poprand = 0;


	Vector3 popPosisin;
};

