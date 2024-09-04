#include "SelectionScenen.h"

SelectionScenen::SelectionScenen()
{
}

SelectionScenen::~SelectionScenen()
{
}

void SelectionScenen::Initialize(){


	worldTransform_1.Initialize();
	worldTransform_2.Initialize();
	worldTransform_3.Initialize();

	camera.Initialize();

	texHandle_ = TextureManager::Load("resources/uvChecker.png");
	ModelManager::LoadObjModel("cube.obj");


	model_1 = std::make_unique<Object3DPlacer>();
	model_1->Initialize();
	model_1->SetModel("cube.obj");
	model_1->SetTexHandle(texHandle_);
	worldTransform_1.translate = { -6,-2,-30 };

	model_2 = std::make_unique<Object3DPlacer>();
	model_2->Initialize();
	model_2->SetModel("cube.obj");
	model_2->SetTexHandle(texHandle_);
	worldTransform_2.translate = { 0,-2,-30 };


	model_3 = std::make_unique<Object3DPlacer>();
	model_3->Initialize();
	model_3->SetModel("cube.obj");
	model_3->SetTexHandle(texHandle_);
	worldTransform_3.translate = { 6,-2,-30 };


	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

	selectCount = 0;

}

void SelectionScenen::Update(){

	worldTransform_1.UpdateMatrix();
	worldTransform_2.UpdateMatrix();
	worldTransform_3.UpdateMatrix();

	camera.UpdateMatrix();


	

	if (input_->PressedKey(DIK_RIGHT)) {
		selectCount++;
	}
	if (input_->PressedKey(DIK_LEFT)) {
		selectCount--;
	}


	if (selectCount == 0) {
		worldTransform_1.rotate.y += 0.2f;
	}
	else {
		worldTransform_1.rotate.y = 0;
	}

	if (selectCount == 1) {
		worldTransform_2.rotate.y += 0.2f;
	}
	else {
		worldTransform_2.rotate.y = 0;
	}

	if (selectCount == 2) {
		worldTransform_3.rotate.y += 0.2f;
	}
	else {
		worldTransform_3.rotate.y = 0;
	}

	if (input_->PressedKey(DIK_SPACE)) {
		sceneNo_ = GAME;
	}

}

void SelectionScenen::Draw(){

	model_1->Draw(worldTransform_1, camera);
	model_2->Draw(worldTransform_2, camera);
	model_3->Draw(worldTransform_3, camera);

}
