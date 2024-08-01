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
	WorldTransform worldTransform1;

	CameraRole camera;


	ModelData model{};
	Animation animation{};
	ModelData model_1{};
//	Animation animation_1{};
//	Skeleton skeleton{};
	std::unique_ptr<Motion> motion_;
	std::unique_ptr<Object3DPlacer> model_;
	//	std::unique_ptr<Matio> matio_1;
	//std::unique_ptr<Model> model_1;

	//プレイヤー
	std::unique_ptr<Player> player_{};

	std::unique_ptr<Skybox> Skybox_;

	uint32_t texHandle_ = 0;
	uint32_t SkyboxTex_ = 0;
	float animationTime = 0.0f;
	float animationTime1 = 0.0f;

	Vector4 color_ = { 1.0f,1.0f,1.0f,1.0f };
	DirectionalLight* directionalLightData_ = nullptr;

};