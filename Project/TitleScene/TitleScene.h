#pragma once
#include "IsScen.h"
#include "Input/Input.h"
#include "Model/Model.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "Sprite/Sprite.h"
#include "Animation/Animation.h"

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
	Model* model_;
	std::unique_ptr<Matio>matio;

};