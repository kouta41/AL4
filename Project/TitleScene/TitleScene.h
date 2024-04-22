#pragma once
#include "IsScen.h"
#include "Input/Input.h"
#include "Model/Model.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "Sprite/Sprite.h"
#include "Animation/Animation.h"
#include "Object3D.h"

class TitleScene : public IScene
{
public:

	TitleScene();
	~TitleScene();

	void Initialize()override;

	void Update()override;

	void Draw()override;

private:
	WorldTransform worldTransform;
	CameraRole camera;

	ModelData model{};
	Animation animation{};
	std::unique_ptr<Matio>matio;
	std::unique_ptr<Object3DPlacer> model_;
	uint32_t texHandle_ = 0;


};