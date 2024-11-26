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
#include "CollisionManager.h"
#include "Particle.h"

#define MAX_PLAYER_CHIPS 5

enum Type {
	EMPTY,///空
	CORE,///核
	CRUST,///外殻
};

enum class Shape {
	shape_I,	//I字ブロック
	shape_T,	//T字ブロック
	shape_S,	//S字ブロック
	shape_O,	//O字ブロック
	shape_J,	//J字ブロック
	shape_L,	//L字ブロック
	shape_ten,	//一つのブロック
	shape_side	//横に連なるブロック
};

struct typeShape final {
	std::vector<std::vector<int32_t>> I;		//I字ブロック
	std::vector<std::vector<int32_t>> T;		//T字ブロック
	std::vector<std::vector<int32_t>> S;		//S字ブロック
	std::vector<std::vector<int32_t>> O;		//O字ブロック
	std::vector<std::vector<int32_t>> J;		//J字ブロック
	std::vector<std::vector<int32_t>> L;		//L字ブロック
	std::vector<std::vector<int32_t>> Ten;		//一つのブロック
	std::vector<std::vector<int32_t>> Side;		//横に連なるブロック
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
	void Initialize(CollisionManager* collisionManager_);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 移動制限
	/// </summary>
	void LimitMove();

	/// <summary>
	/// ブロックの射出
	/// </summary>
	void OutPutBlock();

	/// <summary>
	/// ブロックの形を作る
	/// </summary>
	void BlockShape();

	/// <summary>
	/// 次のブロックの形状
	/// </summary>
	void nextBlockShape();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const CameraRole& viewProjection_);

	//横一列に並んだらブロックが消える
	void OnCollisionLine();

	
	//ワールド座標系を取得
	Vector3 GetWorldPosition();

	//プレイヤーのポジションのセッティング
	void SetplayerLocation_(std::vector<std::vector<int32_t>> playerLocation) { playerLocation_ = playerLocation; }

	//ワールド座標系を取得
	Vector3 GetPlayerCoreWorldPosition();

	//核リストを取得
	const std::list<PlayerCore*>& GetPlayerCores()const { return cores_; }
	//外殻リストを取得
	const std::list<PlayerCrust*>& GetPlayerCrusts()const { return crusts_; }

	float GetClearCount_() { return ClearCount_; }


	// マップの左端
	const float kMapLeftPos = 0.0f;
	// マップの最底辺
	const float kMapBottomPos = -12.0f;
	/// 判定をとるブロックの数
	// 横(ブロックが消えるのに必要な数)
	const int kBlockNumX = 13;
	// 縦
	const int kBlockNumY = 15;

private: // メンバ変数

	WorldTransform worldTransform_;

	WorldTransform nextWorldTransform_[4];

	//パーティクル
	std::unique_ptr<ParticleSystem> particleSystem;
	Emitter emitter;
	std::list<Particle> particles;
	std::mt19937 randomEngine;
	float kDeltaTime = 1.0f / 60.0f;
	bool Particlflag = false;
	float ParticlTime = 0.0f;

	//3Dモデル
	std::unique_ptr<Object3DPlacer> nextmodel_[4];



	//テクスチャハンドル
	uint32_t texHandle_ = 0;
	uint32_t texHandle_1 = 0;
	uint32_t coreTexHandle_ = 0;
	uint32_t crustTexHandle_ = 0;

	std::vector<std::vector<int32_t>> playerLocation_;
	///核
	std::list<PlayerCore*> cores_;
	///外殻
	std::list<PlayerCrust*> crusts_;
	

	Shape ChangeShape_[3];

	//キーボード入力
	Input* input_ = nullptr;

	float ClearCount_ = 0;

	Vector2 clearBlock_[15];

	//フェーズ
	Shape shape_;
	Shape nextShape_;

	typeShape Block;
	// ブロックが消えるフラグ
	bool isDelete_;
	// 当たり判定
	CollisionManager* collisionManager_ = nullptr;

	//移動制限
	float LimitMove_R = 8.0f;
	float LimitMove_L = 8.0f;

	//アウトプットのクールタイム
	float Pushcooltime = 120.f;

};