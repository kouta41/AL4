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

	

private: // メンバ変数
	WorldTransform worldTransform;

	WorldTransform worldTransform_1;
	WorldTransform worldTransform_2;
	WorldTransform worldTransform_3;
	WorldTransform worldTransform_4;
	WorldTransform StartworldTransform_;


	CameraRole camera;

	//プレイヤー
	std::unique_ptr<Player> player_{};
	//敵
	std::unique_ptr<Enemy> enemy_{};

	//天球
	std::unique_ptr<Skydome> skydome_{};

	// 当たり判定
	std::unique_ptr<CollisionManager> collisionManager_{};

	//3Dモデル
	std::unique_ptr<Object3DPlacer> model_1;
	std::unique_ptr<Object3DPlacer> model_2;
	std::unique_ptr<Object3DPlacer> model_3;
	std::unique_ptr<Object3DPlacer> model_4;
	//画面遷移_右
	std::unique_ptr<Object3DPlacer> Endrightmodel_;
	//画面遷移_左
	std::unique_ptr<Object3DPlacer> EndLeftmodel_;
	//スタート
	std::unique_ptr<Object3DPlacer> Startmodel_;

	WorldTransform EndrightworldTransform_;
	WorldTransform EndLeftworldTransform_;

	//キーボード入力
	Input* input_ = nullptr;

	//テクスチャハンドル
	uint32_t texHandle_ = 0;
	uint32_t SkydometexHandle_ = 0;
	uint32_t blacktexHandle_ = 0;
	uint32_t StarttexHandle_ = 0;

	float gameTime = 0;

	bool flag = true;

	bool cameraflag1 = true;
	bool cameraflag2 = true;

	int count_=0;
	Vector3 posA;

	float frame1 = 0.0f;
	float frame2 = 0.0f;

	float endframe1 = 120.0f;
	float endframe2 = 60.0f;

	Vector3 cameraPosA;
	Vector3 cameraRotate;
};