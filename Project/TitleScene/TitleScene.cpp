#include "TitleScene.h"

TitleScene::TitleScene() {}

TitleScene::~TitleScene() {
	//delete sprite_;
}

void TitleScene::Initialize()
{
	camera_.Initialize();
	camera_.translate.z = -20.0f;
	worldTransformCube_.Initialize();
	worldTransformAxis_.Initialize();
	texHandleUV_ = TextureManager::Load("resources/uvChecker.png");
	texHandleMonsterBall_ = TextureManager::Load("resources/monsterBall.png");
	ModelManager::GetInstance()->LoadObjModel("axis.obj");
	ModelManager::GetInstance()->LoadObjModel("cube.obj");


	modelCube_ = ModelManager::GetInstance()->CreateObj("cube.obj");
	modelCube_->SetTexHandle(texHandleUV_);
	modelAxis_ = ModelManager::GetInstance()->CreateObj("axis.obj");
	modelAxis_->SetTexHandle(texHandleUV_);
	worldTransformCube_.translate = { 5.0f,0.0f,0.0f };
	worldTransformAxis_.translate = { -3.0f,0.0f,0.0f };

	// particle
	randomEngine_ = particle1_->random();
	emit.count = 3;
	emit.frequency = 0.5f;
	emit.frequencyTime = 0.0f;
	particle2_ = std::make_unique<ParticleSystem>();
	particle2_->Initialize("cube.obj");
	particle2_->SetTexHandle(texHandleMonsterBall_);
	particle1_ = std::make_unique<ParticleSystem>();
	particle1_->Initialize("cube.obj");
	particle1_->SetTexHandle(texHandleUV_);
	accelerationField.acceleration = { 15.0f, 0.0f,0.0f };
	accelerationField.area.min = { -1.0f,-1.0f,-1.0f };
	accelerationField.area.max = { 1.0f,1.0f,1.0f };

}

void TitleScene::Update() {	


	ImGui::Begin("Camera");

	if (ImGui::TreeNode("transform")) {
		ImGui::SliderAngle("rotateX", &camera_.rotate.x);
		ImGui::SliderAngle("rotateY", &camera_.rotate.y);
		ImGui::SliderAngle("rotateZ", &camera_.rotate.z);
		ImGui::DragFloat3("translate", &camera_.translate.x, 0.01f);
		ImGui::DragFloat3("scale", &camera_.scale.x, 0.01f);
		ImGui::TreePop();
	}
	ImGui::End();

	ImGui::Begin("particle");
	ImGui::Checkbox("DrawParticle1", &isDrawParticle1_);
	ImGui::Checkbox("DrawParticle2", &isDrawParticle2_);
	ImGui::End();

	ImGui::Begin("SceneChang");
	ImGui::Text("SPACE KEY");
	ImGui::End();
	

	if (input_->PushKey(DIK_SPACE)) {
		sceneNo_ = GAME;
	}

	/*----------------------------
			パーティクル
	------------------------------*/
	emit.frequencyTime += dt;
	if (emit.frequency <= emit.frequencyTime) {
		particles1_.splice(particles1_.end(), particle1_->Emission(emit, randomEngine_));
		particles2_.splice(particles2_.end(), particle2_->Emission(emit, randomEngine_));
		emit.frequencyTime -= emit.frequency;
	}


	for (std::list<Particle>::iterator particleItr = particles1_.begin();
		particleItr != particles1_.end(); ++particleItr) {

		if (particle1_->IsCollision(accelerationField.area, (*particleItr).worldTransform.translate)) {
			(*particleItr).velocity = Add((*particleItr).velocity, Multiply(dt, accelerationField.acceleration));
		}

		(*particleItr).worldTransform.translate = Add((*particleItr).worldTransform.translate, Multiply(dt, (*particleItr).velocity));
		(*particleItr).worldTransform.UpdateMatrix();

	}

	for (std::list<Particle>::iterator particleItr2 = particles2_.begin();
		particleItr2 != particles2_.end(); ++particleItr2) {

		if (particle2_->IsCollision(accelerationField.area, (*particleItr2).worldTransform.translate)) {
			(*particleItr2).velocity = Add((*particleItr2).velocity, Multiply(dt, accelerationField.acceleration));
		}

		(*particleItr2).worldTransform.translate = Add((*particleItr2).worldTransform.translate, Multiply(dt, (*particleItr2).velocity));
		(*particleItr2).worldTransform.UpdateMatrix();

	}

	
	camera_.UpdateMatrix();
	worldTransformCube_.UpdateMatrix();
	worldTransformAxis_.UpdateMatrix();

}

void TitleScene::Draw()
{

	modelCube_->Draw(worldTransformCube_, camera_);
	modelAxis_->Draw(worldTransformAxis_, camera_);

	if (isDrawParticle1_) {
		particle1_->Draw(particles1_, camera_);
	}

	if (isDrawParticle2_) {
		particle2_->Draw(particles2_, camera_);
	}
}