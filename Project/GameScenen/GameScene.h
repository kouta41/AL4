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


/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

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
	/// 衝突判定
	/// </summary>
	void CheckAllcollisons();
private: // メンバ変数

	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
	//3Dモデル
	std::unique_ptr<Model> model_{};
	//プレイヤー
	std::unique_ptr<Player> player_{};
	//敵
	std::unique_ptr<Enemy> enemy_{};

	uint32_t texHandle_ = 0;
};