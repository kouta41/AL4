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
	void Draw(CameraRole viewProjection_);

	//
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
	const int kBlockNumX = 9;
	// 縦
	const int kBlockNumY = 15;

private: // メンバ変数

	WorldTransform worldTransform_;

	WorldTransform worldTransform_1;
	WorldTransform worldTransform_2;
	WorldTransform worldTransform_3;
	WorldTransform worldTransform_4;

	//3Dモデル
	std::unique_ptr<Object3DPlacer> model_1;
	std::unique_ptr<Object3DPlacer> model_2;
	std::unique_ptr<Object3DPlacer> model_3;
	std::unique_ptr<Object3DPlacer> model_4;

	std::unique_ptr<Object3DPlacer> Startmodel_;


	//テクスチャハンドル
	uint32_t texHandle_ = 0;
	uint32_t texHandle_1 = 0;


	std::vector<std::vector<int32_t>> playerLocation_;
	///核
	std::list<PlayerCore*> cores_;
	///外殻
	std::list<PlayerCrust*> crusts_;
	

	Shape ChangeShape_[3];


	Vector3 position_;
	
	//速度
	Vector3 velocity_;


	int popplayerPosition_ = 0;

	int popCountCore_ = 0;

	int popCountcrust_ = 0;
	//キーボード入力
	Input* input_ = nullptr;

	//テクスチャハンドル
	uint32_t coreTexHandle_ = 0;
	uint32_t crustTexHandle_ = 0;

	
	float ClearCount_ = 0;

	Vector2 clearBlock_[15];

	//フェーズ
	Shape shape_;

	typeShape Block;
	// ブロックが消えるフラグ
	bool isDelete_;
	// 当たり判定
	CollisionManager* collisionManager_ = nullptr;

	//移動制限
	float LimitMove = 8.0f;
};