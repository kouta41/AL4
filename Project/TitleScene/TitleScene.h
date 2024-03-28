#pragma once
#include "IsScen.h"
#include "Input.h"
#include "Model.h"
#include "TextureManager.h"
#include "Sprite.h"

class TitleScene : public IScene
{
public:

	TitleScene();
	~TitleScene();

	void Initialize()override;

	void Update()override;

	void Draw()override;

private:
	uint32_t texhandle_ = 0;
	uint32_t texhandle1_ = 0;
	uint32_t texhandle2_ = 0;
	//ワールド変換データ
	WorldTransform worldTransform_;
	WorldTransform worldTransform1_;

	ViewProjection viewProjection_{};
	ViewProjection viewProjection1_{};

	std::unique_ptr<Sprite> sprite_;
	std::unique_ptr<Sprite> sprite1_;
	std::unique_ptr<Sprite> sprite2_;

	Vector2 pos = { 640.0f,360.0f };
	float time = 0.0f;
	float i = 0.1f;

	float color = 0.0f;

	bool flag = true;
	//キーボード入力
	Input* input_ = nullptr;
};