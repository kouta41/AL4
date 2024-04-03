#pragma once
#include "Sprite.h"
#include "Triangle.h"
#include "Model.h"
#include "ModelSphere.h"
#include "ModelCube.h"
#include "Input.h"
#include "ImGuiManager/ImGuiManager.h"
#include "CollisionManager.h"
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


private: // メンバ変数

	WorldTransform worldTransform_;
	CameraRole viewProjection_;

	uint32_t texHandleUV_ = 0;
	uint32_t texHandleCircle_ = 0;



	std::unique_ptr<Sprite> spriteUV_ = nullptr;
	std::unique_ptr<Sprite> spriteCircle_ = nullptr;


	//キーボード入力
	Input* input_ = nullptr;

	Vector2 pos = { 1.0f,1.0f };

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	void CheckAllCollisions();
};