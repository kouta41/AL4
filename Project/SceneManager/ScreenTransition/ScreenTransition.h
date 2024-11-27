#pragma once
/// <summary>
/// 画面遷移のオブジェクト
/// </summary>
#include "IsScen.h"
#include "Animation.h"
#include "Skybox.h"
#include "BlockManager.h"
#include "Enemy.h"
#include "Skydome.h"
#include "CollisionManager.h"
#include "Audio.h"


class ScreenTransition{

public:
	ScreenTransition();
	~ScreenTransition();

	void Initialize();

	void SetStartWorld();

	void SetEndWorld();

	void SetGameWorld();

	void Update();

	void StartMove();

	void EndMove();

	void Draw(CameraRole camera);


	//ワールド座標のGetter
	WorldTransform GetworldTransform_R() { return worldTransform_R; }
	WorldTransform GetworldTransform_L() { return worldTransform_L; }


private:
	//ワールド座標
	WorldTransform worldTransform_R;
	WorldTransform worldTransform_L;
	


	//音
	Audio* audio_ = Audio::GetInstance();

	//画面遷移_右
	std::unique_ptr<Object3DPlacer> rightmodel_;
	//画面遷移_左
	std::unique_ptr<Object3DPlacer> Leftmodel_;

	//テクスチャハンドル
	uint32_t blacktexHandle_ = 0;
	//SEの保管
	uint32_t sceneSE = 0;

	bool shiftSceneFlag = true;
};

