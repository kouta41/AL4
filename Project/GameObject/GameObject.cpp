#include "GameObject.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

void GameObject::Initialize(){
	//ワールド座標の初期化
	worldTransform_Right.Initialize();
	worldTransform_Left.Initialize();
	worldTransform_Bottom.Initialize();
	
	//テクスチャのロード
	texHandle_ = TextureManager::Load("resources/white.png");

	//ステージ右、モデルの初期化
	Stagemodel_Right = std::make_unique<Object3DPlacer>();
	Stagemodel_Right->Initialize();
	Stagemodel_Right->SetModel("cube.obj");
	Stagemodel_Right->SetTexHandle(texHandle_);
	worldTransform_Right.translate = { 14,0,0 };
	worldTransform_Right.scale = { 1,15,1 };

	//ステージ左、モデルの初期化
	Stagemodel_Left = std::make_unique<Object3DPlacer>();
	Stagemodel_Left->Initialize();
	Stagemodel_Left->SetModel("cube.obj");
	Stagemodel_Left->SetTexHandle(texHandle_);
	worldTransform_Left.translate = { -14,0,0 };
	worldTransform_Left.scale = { 1,15,1 };

	//ステージ下、モデルの初期化
	Stagemodel_Bottom = std::make_unique<Object3DPlacer>();
	Stagemodel_Bottom->Initialize();
	Stagemodel_Bottom->SetModel("cube.obj");
	Stagemodel_Bottom->SetTexHandle(texHandle_);
	worldTransform_Bottom.translate = { 0,-14,0 };
	worldTransform_Bottom.scale = { 17,1,1 };

}

void GameObject::Update(){
	//ワールド座標の更新
	worldTransform_Right.UpdateMatrix();
	worldTransform_Left.UpdateMatrix();
	worldTransform_Bottom.UpdateMatrix();
}

void GameObject::Draw(CameraRole viewProjection_){

#ifdef _DEBUG
	ImGui::Begin("GameObject");
	if (ImGui::TreeNode("EndrightworldTransform_")) {
		ImGui::DragFloat3("translate", &worldTransform_Right.translate.x, 0.1f, 100, 100);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("EndLeftworldTransform_")) {
		ImGui::DragFloat3("translate", &worldTransform_Left.translate.x, 0.1f, 100, 100);
		ImGui::TreePop();
	}
	ImGui::End();
#endif // RELEASE
	//描画
	Stagemodel_Right->Draw(worldTransform_Right, viewProjection_);
	Stagemodel_Left->Draw(worldTransform_Left, viewProjection_);
	Stagemodel_Bottom->Draw(worldTransform_Bottom, viewProjection_);
}
