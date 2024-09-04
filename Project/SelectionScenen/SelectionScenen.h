#pragma once

#include "IsScen.h"
#include "Input/Input.h"
#include "Model/Model.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "Sprite/Sprite.h"
#include "Object3D.h"
#include "Animation/Animation.h"

class SelectionScenen : public IScene{
public:

	SelectionScenen();
	~SelectionScenen();

	void Initialize()override;

	void Update()override;

	void Draw()override;


	int GetselectCount()  {return selectCount; }

private:
	WorldTransform worldTransform_1;
	WorldTransform worldTransform_2;
	WorldTransform worldTransform_3;

	CameraRole camera;

	std::unique_ptr<Sprite> sprite_;
	//3Dモデル
	std::unique_ptr<Object3DPlacer> model_1;
	std::unique_ptr<Object3DPlacer> model_2;
	std::unique_ptr<Object3DPlacer> model_3;


	int selectCount = 0;

	//キーボード入力
	Input* input_ = nullptr;

	//テクスチャハンドル
	uint32_t texHandle_ = 0;

};

