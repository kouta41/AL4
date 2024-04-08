#pragma once
#include "Sprite.h"
#include "Triangle.h"
#include "Model.h"
#include "ModelSphere.h"
#include "ModelCube.h"
#include "Input.h"
#include "ImGuiManager/ImGuiManager.h"
#include "Player.h"
#include "Enemy.h"
#include "CollisionManager.h"
#include "Skydome.h"
#include "RailCamera.h"
#include "EnemyBullet.h"
#include "IsScen.h"
#include <list>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene : public IScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

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
	void Draw();
	/// <summary>
		/// 敵発生データの読み込み
		/// </summary>
	void LoadEnemyPopData();

	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdateEnemyPopCommands();

	void EnemyInitialize(Vector3);

	/// <summary>
	///	敵:敵弾を追加する
	/// </summary>
	void AddEnemyBullet(EnemyBullet* enemyBullet);
	void AddEnemy(Enemy* enemy);

private: // メンバ変数

	WorldTransform worldTransform_;
	CameraRole viewProjection_;
	//3Dモデル
	std::unique_ptr<Model> model_{};
	//プレイヤー
	std::unique_ptr<Player> player_{};
#pragma region Enemy
	//敵
	std::list<Enemy*> enemys_;
	// 待機タイマー
	int32_t waitTimer;
	// 待機フラグ
	bool isWait = false;
	//敵の弾リスト
	std::list<EnemyBullet*> enemyBullets_;
	//敵発生コマンド
	std::stringstream enemyPopCommands;
	
#pragma endregion 敵

	//天球
	std::unique_ptr<Skydome> skydome_{};
	//3Dモデル
	std::unique_ptr<Model> modelSkydome_{};
	//レールカメラ
	std::unique_ptr<RailCamera> railCamera_{};
	//テクスチャ
	uint32_t texHandleSkydome_ = 0;
	uint32_t texHandle_ = 0;

	//CorisionManager
	CollisionManager* collisionManager_;

	std::unique_ptr<Sprite> sprite2_;
	uint32_t texhandle2_ = 0;
	float time = 0.0f;
	float i = 0.1f;
	float color = 2.0f;
	bool flag = true;
	/// <summary>
	/// ゲームシーン用
	/// </summary>
	void CheckAllCollisions();
};