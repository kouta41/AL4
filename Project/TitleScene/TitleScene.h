#pragma once
#include "IsScen.h"
#include "Input.h"
#include "Model.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include "Sprite.h"
#include "Particle/Particle.h"
#include "Mathfunction.h"

class TitleScene : public IScene
{
public:

	TitleScene();
	~TitleScene();

	void Initialize()override;

	void Update()override;

	void Draw()override;

private:
	CameraRole camera_{};
	WorldTransform worldTransformCube_{};
	WorldTransform worldTransformAxis_{};
	uint32_t texHandleUV_ = 0;
	uint32_t texHandleMonsterBall_ = 0;
	Model* modelCube_ = nullptr;
	Model* modelAxis_ = nullptr;

	// particle
	std::list<Particle> particles1_ = {};
	std::list<Particle> particles2_ = {};
	std::unique_ptr<ParticleSystem> particle1_ = {};
	std::unique_ptr<ParticleSystem> particle2_ = {};
	const float dt = 1.0f / 60.0f;
	Emitter emit{};
	AccelerationField accelerationField{};
	std::mt19937 randomEngine_;
	bool isDrawParticle1_ = false;
	bool isDrawParticle2_ = false;

	//キーボード入力
	Input* input_ = nullptr;
};