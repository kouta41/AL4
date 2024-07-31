#pragma once
#include "IsScen.h"
#include "Input/Input.h"
#include "Model/Model.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "Sprite/Sprite.h"
#include "Object3D.h"
#include "Animation/Animation.h"
#include "Skybox/Skybox.h"
#include "Player.h"
#include "Enemy.h"
#include "Skydome.h"
#include "CollisionManager.h"

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


	void Initialize()override;

	void Update()override;

	void Draw()override;

	/// <summary>
	///	当たり判定
	/// </summary>
	void CheckAllCollisions();

private: // メンバ変数
	WorldTransform worldTransform;

	CameraRole camera;

	//プレイヤー
	std::unique_ptr<Player> player_{};
	//敵
	std::unique_ptr<Enemy> enemy_{};

	//天球
	std::unique_ptr<Skydome> skydome_{};


	//CorisionManager
	CollisionManager* collisionManager_;

	//キーボード入力
	Input* input_ = nullptr;

	//テクスチャハンドル
	uint32_t texHandle_ = 0;
	uint32_t SkydometexHandle_ = 0;

	float gameTime = 0;


};