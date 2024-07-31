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

	WorldTransform spriteWorldTransform;

	CameraRole camera;

	bool flag = true;
	std::unique_ptr<Sprite> sprite_;
	std::unique_ptr<Sprite> sprite_1;

	Vector2 pos = { 0,0 };

	//キーボード入力
	Input* input_ = nullptr;

	//テクスチャハンドル
	uint32_t texHandle_ = 0;
	uint32_t texHandle_1 = 0;

	uint32_t SkydometexHandle_ = 0;

};