#pragma once

#include "IsScen.h"
#include "Input/Input.h"
#include "Model/Model.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "Sprite/Sprite.h"
#include "Object3D.h"
#include "Animation/Animation.h"

/// <summary>
/// ステージ選択シーン
/// </summary>
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

	WorldTransform EndrightworldTransform_;
	WorldTransform EndLeftworldTransform_;

	CameraRole camera;

	std::unique_ptr<Sprite> sprite_;
	///3Dモデル
	std::unique_ptr<Object3DPlacer> model_1;
	std::unique_ptr<Object3DPlacer> model_2;
	std::unique_ptr<Object3DPlacer> model_3;
	//画面遷移_右
	std::unique_ptr<Object3DPlacer> Endrightmodel_;
	//画面遷移_左
	std::unique_ptr<Object3DPlacer> EndLeftmodel_;

	int selectCount = 0;

	bool flag = true;

	//キーボード入力
	Input* input_ = nullptr;

	//テクスチャハンドル
	uint32_t texHandle_ = 0;
	uint32_t blacktexHandle_ = 0;


};

