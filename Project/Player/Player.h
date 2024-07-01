#pragma once
#include <list>

#include "Sprite.h"
#include "Triangle.h"
#include "Model.h"
#include "ModelSphere.h"
#include "ModelCube.h"
#include "Input.h"
#include "ImGuiManager/ImGuiManager.h"
#include "PlayerCrust.h"
#include "PlayerCore.h"
#include "Collider.h"
#include "CollisionConfig.h"
#include "ModelManager.h"


#define MAX_PLAYER_CHIPS 7

enum Type {
	EMPTY,///空
	CORE,///核
	CRUST,///外殻
};


/// <summary>
/// プレイヤー
/// </summary>
class Player:public Collider {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	Player();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

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
	void Draw(CameraRole viewProjection_);

	//衝突判定
	void OnCollision();

	
	//ワールド座標系を取得
	Vector3 GetWorldPosition();

	//プレイヤーのポジションのセッティング
	void SetplayerLocation_(std::vector<std::vector<int32_t>> playerLocation) { playerLocation_ = playerLocation; }

private: // メンバ変数

	WorldTransform worldTransform_;
	CameraRole viewProjection_;

	std::vector<std::vector<int32_t>> playerLocation_;
	///核
	std::list<PlayerCore*> cores_;
	///外殻
	std::list<PlayerCrust*> crusts_;
	

	Vector3 position_;
	

	WorldTransform worldTransform;

	//速度
	Vector3 velocity_;


	int popplayerPosition_ = 0;

	int popCountCore_ = 0;

	int popCountcrust_ = 0;
	//キーボード入力
	Input* input_ = nullptr;

	//テクスチャハンドル
	uint32_t texHandle_ = 0;
	uint32_t texHandleBullet_ = 0;
	uint32_t textureReticle_ = 0;
	
};