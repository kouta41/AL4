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
#include "Audio/Audio.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene : public IScene
{
public:

	TitleScene();
	~TitleScene();

	void Initialize()override;

	void Update()override;

	void Draw()override;

	/// <summary>
	///	当たり判定
	/// </summary>
	void CheckAllCollisions();
private:
	WorldTransform worldTransform;
	WorldTransform TitleworldTransform_;
	WorldTransform objectworldTransform_R; 
	WorldTransform objectworldTransform_L;
	WorldTransform StartworldTransform_;

	WorldTransform EndrightworldTransform_;
	WorldTransform EndLeftworldTransform_;

	WorldTransform spriteWorldTransform;

	CameraRole camera;


	Audio* audio_ = Audio::GetInstance();


	bool flag = true;
	std::unique_ptr<Sprite> sprite_;
	std::unique_ptr<Sprite> blackSprite_;

	Vector2 pos = { 0,0 };

	///3Dモデル
	//タイトル
	std::unique_ptr<Object3DPlacer> Titlemodel_;
	//オブジェクト_R
	std::unique_ptr<Object3DPlacer> objectemodel_R;
	//オブジェクト_L
	std::unique_ptr<Object3DPlacer> objectemodel_L;
	//スタート
	std::unique_ptr<Object3DPlacer> Startmodel_;
	//画面遷移_右
	std::unique_ptr<Object3DPlacer> Endrightmodel_;
	//画面遷移_左
	std::unique_ptr<Object3DPlacer> EndLeftmodel_;
	//天球
	std::unique_ptr<Skydome> skydome_{};


	//キーボード入力
	Input* input_ = nullptr;

	//テクスチャハンドル
	uint32_t TitletexHandle_ = 0;
	uint32_t objecttexHandle_R = 0;
	uint32_t objecttexHandle_L = 0;
	uint32_t StarttexHandle_ = 0;

	uint32_t SkydometexHandle_ = 0;
	uint32_t blacktexHandle_ = 0;
	

	uint32_t sceneBGM = 0;
	uint32_t sceneSE = 0;

	float Color_R = 1.0f;
	float Color_L = 1.0f;
	float speed = 0.06f;
};