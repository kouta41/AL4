/// <summary>
/// タイトルシーン
/// </summary>
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
#include "BlockManager.h"
#include "Skydome.h"
#include "CollisionManager.h"
#include "Audio/Audio.h"
#include "BlockCore.h"
#include "GlobalVariables.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

class TitleScene : public IScene
{
public:

	TitleScene();
	~TitleScene();

	void Initialize()override;

	void Update()override;

	void Draw()override;

	//調整項目の適任
	void ApplyGlobalVariaBles();


private:

	//ワールド座標
	WorldTransform worldTransform;
	WorldTransform TitleworldTransform_;
	
	WorldTransform StartworldTransform_;

	WorldTransform EndrightworldTransform_;
	WorldTransform EndLeftworldTransform_;

	WorldTransform spriteWorldTransform;

	//カメラ座標
	CameraRole camera;

	//オーディオ
	Audio* audio_ = Audio::GetInstance();

	//調整項目
	GlobalVariables* globalVariables_;

	//画面遷移のフラグ
	bool TransitionFlag = true;

	//スプライト
	std::unique_ptr<Sprite> sprite_;
	std::unique_ptr<Sprite> blackSprite_;


	//落ちてくるブロックの位置
	Vector3 randPos_;
	//落ちてくるタイミング
	float foolTime = 0;

	//落ちてくるタイミング
	float foolTime1 = 0;

	///3Dモデル
	//タイトル
	std::unique_ptr<Object3DPlacer> Titlemodel_;
	
	//スタート
	std::unique_ptr<Object3DPlacer> Startmodel_;
	//画面遷移_右
	std::unique_ptr<Object3DPlacer> Endrightmodel_;
	//画面遷移_左
	std::unique_ptr<Object3DPlacer> EndLeftmodel_;
	//天球
	std::unique_ptr<Skydome> skydome_{};

	//落ちてくるブロック
	std::list<BlockCore*> cores_;

	//キーボード入力
	Input* input_ = nullptr;

	//テクスチャハンドル
	uint32_t TitletexHandle_ = 0;
	uint32_t StarttexHandle_ = 0;

	uint32_t SkydometexHandle_ = 0;
	uint32_t blacktexHandle_ = 0;
	
	uint32_t coreTexHandle_ = 0;
	uint32_t sceneBGM = 0;
	uint32_t sceneSE = 0;

	//グループ名
	const char* groupName = "TitleObject";


	float Color_R = 1.0f;
	float Color_L = 1.0f;
	float speed = 0.06f;
};