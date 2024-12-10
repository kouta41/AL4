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
	texHandle_UI= TextureManager::Load("resources/8qrp7.png");
	texHandle_chage =TextureManager::Load("resources/rgnk7.png");
	texHandle_ON = TextureManager::Load("resources/5030v.png");
	texHandle_OFF = TextureManager::Load("resources/396iw.png");
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


	Sprite::StaticInitialize();
	spriteWorldTransform.Initialize();
	spriteWorldTransform_chage.Initialize();
	spriteWorldTransform_ON.Initialize();
	spriteWorldTransform_OFF.Initialize();


	UIsprite_.reset(Sprite::Create(texHandle_UI, { 0,0 }));

	UIsprite_chage.reset(Sprite::Create(texHandle_chage, { 0,100 }));
	spriteWorldTransform_chage.translate = { 0,50,0 };
	UIsprite_chage->SetWorldTransform(spriteWorldTransform_chage);

	UIsprite_ON.reset(Sprite::Create(texHandle_ON, { 200,100 }));
	spriteWorldTransform_ON.translate = { 230,50,0 };
	UIsprite_ON->SetWorldTransform(spriteWorldTransform_ON);

	UIsprite_OFF.reset(Sprite::Create(texHandle_OFF, {200,100 }));
	spriteWorldTransform_OFF.translate = { 230,50,0 };
	UIsprite_OFF->SetWorldTransform(spriteWorldTransform_ON);


}

void GameObject::Update(){
	//ワールド座標の更新
	worldTransform_Right.UpdateMatrix();
	worldTransform_Left.UpdateMatrix();
	worldTransform_Bottom.UpdateMatrix();
	Sprite::StaticUpdate();

	spriteWorldTransform.UpdateMatrix();
	//spriteWorldTransform = UIsprite_->GetWorldTransform();
//	UIsprite_->SetWorldTransform(spriteWorldTransform);


}

void GameObject::Draw(CameraRole viewProjection_){

#ifdef _DEBUG
	ImGui::Begin("spriteWorldTransform");
	if (ImGui::TreeNode("spriteWorldTransform")) {
		ImGui::DragFloat3("translate", &spriteWorldTransform.translate.x, 0.1f, 100, 100);
		ImGui::DragFloat3("rotate", &spriteWorldTransform.rotate.x, 0.1f, 100, 100);
		ImGui::DragFloat3("scale", &spriteWorldTransform.scale.x, 0.1f, 100, 100);


		ImGui::TreePop();
	}
	ImGui::End();
#endif // RELEASE
	//描画
	Stagemodel_Right->Draw(worldTransform_Right, viewProjection_);
	Stagemodel_Left->Draw(worldTransform_Left, viewProjection_);
	Stagemodel_Bottom->Draw(worldTransform_Bottom, viewProjection_);
}

void GameObject::Draw2D(CameraRole viewProjection_){
	UIsprite_->Draw();
	UIsprite_chage->Draw();

	if (blockManager_->GetIscollision_()) {
		UIsprite_OFF->Draw();
	}
	else {
		UIsprite_ON->Draw();
	}
}
