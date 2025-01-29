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
	worldTransform_background.Initialize();
	worldTransform_guide.Initialize();
	
	//テクスチャのロード
	texHandle_ = TextureManager::Load("resources/1706.png");
	texHandle_UI= TextureManager::Load("resources/8qrp7.png");
	texHandle_chage =TextureManager::Load("resources/rgnk7.png");
	texHandle_ON = TextureManager::Load("resources/5030v.png");
	texHandle_OFF = TextureManager::Load("resources/396iw.png");
	texHandle_Block = TextureManager::Load("resources/cube.jpg");
	texHandle_background= TextureManager::Load("resources/104696.jpg");
	texHandle_guide = TextureManager::Load("resources/white.png");

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


	//ステージの背景
	backgroundModel_ = std::make_unique<Object3DPlacer>();
	backgroundModel_->Initialize();
	backgroundModel_->SetModel("cube.obj");
	backgroundModel_->SetTexHandle(texHandle_background);
	worldTransform_background.translate = { 4.0f,0.0f,1.0f };
	worldTransform_background.scale = { 24.0f,17.0f,0.0f };

	//ブロックのガイドライン
	guideModel_ = std::make_unique<Object3DPlacer>();
	guideModel_->Initialize();
	guideModel_->SetModel("cube.obj");
	guideModel_->SetTexHandle(texHandle_guide);
	worldTransform_guide.translate = {1.0f,1.0f,1.0f  };
	worldTransform_guide.scale = { 1.0f,1.0f,0.0f };


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


	for (int i = 0; i < 4; i++) {
		nextWorldTransform_[i].Initialize();
		scandWorldTransform_[i].Initialize();

		nextWorldTransform_[i].rotate.y = 0.2f;

		scandWorldTransform_[i].rotate.y = 0.2f;
		nextModel_[i]= std::make_unique<Object3DPlacer>();
		nextModel_[i]->Initialize();
		nextModel_[i]->SetModel("cube.obj");
		nextModel_[i]->SetTexHandle(texHandle_Block);

		scandModel_[i] = std::make_unique<Object3DPlacer>();
		scandModel_[i]->Initialize();
		scandModel_[i]->SetModel("cube.obj");
		scandModel_[i]->SetTexHandle(texHandle_Block);
	}

}

void GameObject::Update(){
	//ワールド座標の更新
	worldTransform_Right.UpdateMatrix();
	worldTransform_Left.UpdateMatrix();
	worldTransform_Bottom.UpdateMatrix();
	worldTransform_background.UpdateMatrix();
	worldTransform_guide.UpdateMatrix();

	Sprite::StaticUpdate();

	spriteWorldTransform.UpdateMatrix();


	for (int i = 0; i < 4; i++) {
		nextWorldTransform_[i].UpdateMatrix();
		scandWorldTransform_[i].UpdateMatrix();
	}


	//ブロックの状況の共有
	nextShape[0] = blockManager_->GetChangeShape();
	nextShape[1] = blockManager_->GetChangeShape1();
	nextShape[2] = blockManager_->GetChangeShape2();


	/*-----------------------------------------------------------------------

	今のブロック

	--------------------------------------------------------------------------*/

	switch (nextShape[0])
	{
	case Shape::shape_I:
		worldTransform_guide.translate = {
				blockManager_->GetWorldPosition().x,
				blockManager_->GetWorldPosition().y - 11,
				1.0f
		};

		worldTransform_guide.scale = { 1.0f,16.0f,0.0f };

		break;

	case Shape::shape_T:
		worldTransform_guide.translate = {
			blockManager_->GetWorldPosition().x,
			blockManager_->GetWorldPosition().y-11,
			1.0f
		};

		worldTransform_guide.scale = { 3.0f,16.0f,0.0f };

		break;

	case Shape::shape_S:
		worldTransform_guide.translate = {
			blockManager_->GetWorldPosition().x,
			blockManager_->GetWorldPosition().y - 11,
			1.0f
		};

		worldTransform_guide.scale = { 3.0f,16.0f,0.0f };

		break;

	case Shape::shape_O:
		worldTransform_guide.translate = {
				blockManager_->GetWorldPosition().x+1,
				blockManager_->GetWorldPosition().y - 11,
				1.0f
		};

		worldTransform_guide.scale = { 2.0f,16.0f,0.0f };

		break;

	case Shape::shape_J:
		worldTransform_guide.translate = {
					blockManager_->GetWorldPosition().x - 1,
					blockManager_->GetWorldPosition().y - 11,
					1.0f
		};

		worldTransform_guide.scale = { 2.0f,16.0f,0.0f };
		break;

	case Shape::shape_L:
		worldTransform_guide.translate = {
				blockManager_->GetWorldPosition().x + 1,
				blockManager_->GetWorldPosition().y - 11,
				1.0f
		};

		worldTransform_guide.scale = { 2.0f,16.0f,0.0f };

		break;
	case Shape::shape_ten:
		worldTransform_guide.translate = {
				blockManager_->GetWorldPosition().x ,
				blockManager_->GetWorldPosition().y - 11,
				1.0f
		};

		worldTransform_guide.scale = { 1.0f,16.0f,0.0f };

		break;
	case Shape::shape_side:
		worldTransform_guide.translate = {
				blockManager_->GetWorldPosition().x + 1,
				blockManager_->GetWorldPosition().y - 11,
				1.0f
		};

		worldTransform_guide.scale = { 2.0f,16.0f,0.0f };
		break;
	}


	/*-----------------------------------------------------------------------

	次のブロック

	--------------------------------------------------------------------------*/
	switch (nextShape[1])
	{
	case Shape::shape_I:
		//ブロック1
		nextWorldTransform_[0].translate.x = 22.0f;
		nextWorldTransform_[0].translate.y = 8.0f;
		//ブロック2
		nextWorldTransform_[1].translate.x = 22.0f;
		nextWorldTransform_[1].translate.y = 6.0f;
		//ブロック3
		nextWorldTransform_[2].translate.x = 22.0f;
		nextWorldTransform_[2].translate.y = 4.0f;
		//ブロック4
		nextWorldTransform_[3].translate.x = 22.0f;
		nextWorldTransform_[3].translate.y = 2.0f;

		break;

	case Shape::shape_T:
		//ブロック1
		nextWorldTransform_[0].translate.x = 22.0f;
		nextWorldTransform_[0].translate.y = 8.0f;
		//ブロック2
		nextWorldTransform_[1].translate.x = 20.f;
		nextWorldTransform_[1].translate.y =8.0f;
		//ブロック3
		nextWorldTransform_[2].translate.x = 24.0f;;
		nextWorldTransform_[2].translate.y = 8.0f;
		//ブロック4
		nextWorldTransform_[3].translate.x = 22.0f;
		nextWorldTransform_[3].translate.y = 6.0f;


		break;

	case Shape::shape_S:
		//ブロック1
		nextWorldTransform_[0].translate.x = 22.0f;
		nextWorldTransform_[0].translate.y = 8.0f;
		//ブロック2
		nextWorldTransform_[1].translate.x = 24.0f;
		nextWorldTransform_[1].translate.y = 8.0f;;
		//ブロック3
		nextWorldTransform_[2].translate.x = 22.0f;
		nextWorldTransform_[2].translate.y = 6.0f;
		//ブロック4
		nextWorldTransform_[3].translate.x = 20.0f;
		nextWorldTransform_[3].translate.y = 6.0f;;

		
		break;

	case Shape::shape_O:
		//ブロック1
		nextWorldTransform_[0].translate.x = 22.0f;
		nextWorldTransform_[0].translate.y = 8.0f;
		//ブロック2
		nextWorldTransform_[1].translate.x = 24.0f;
		nextWorldTransform_[1].translate.y = 8.0f;
		//ブロック3
		nextWorldTransform_[2].translate.x = 22.0f;
		nextWorldTransform_[2].translate.y = 6.0f;
		//ブロック4
		nextWorldTransform_[3].translate.x = 24.0f;
		nextWorldTransform_[3].translate.y = 6.0f;


		break;

	case Shape::shape_J:
		//ブロック1
		nextWorldTransform_[0].translate.x = 22.0f;
		nextWorldTransform_[0].translate.y = 8.0f;
		//ブロック2
		nextWorldTransform_[1].translate.x = 22.0f;
		nextWorldTransform_[1].translate.y = 6.0f;
		//ブロック3
		nextWorldTransform_[2].translate.x = 22.0f;
		nextWorldTransform_[2].translate.y = 4.0f;
		//ブロック4
		nextWorldTransform_[3].translate.x = 20.0f;
		nextWorldTransform_[3].translate.y = 4.0f;


		break;

	case Shape::shape_L:
		//ブロック1
		nextWorldTransform_[0].translate.x = 22.0f;
		nextWorldTransform_[0].translate.y = 8.0f;
		//ブロック2
		nextWorldTransform_[1].translate.x = 22.0f;
		nextWorldTransform_[1].translate.y = 6.0f;
		//ブロック3
		nextWorldTransform_[2].translate.x = 22.0f;
		nextWorldTransform_[2].translate.y = 4.0f;
		//ブロック4
		nextWorldTransform_[3].translate.x = 24.0f;
		nextWorldTransform_[3].translate.y = 4.0f;


		break;
	case Shape::shape_ten:
		//ブロック1
		nextWorldTransform_[0].translate.x = 22.0f;
		nextWorldTransform_[0].translate.y = 8.0f;
		//ブロック2
		nextWorldTransform_[1].translate.x = 22.0f;
		nextWorldTransform_[1].translate.y = 8.0f;
		//ブロック3
		nextWorldTransform_[2].translate.x = 22.0f;
		nextWorldTransform_[2].translate.y = 8.0f;
		//ブロック4
		nextWorldTransform_[3].translate.x = 22.0f;
		nextWorldTransform_[3].translate.y = 8.0f;
		break;
	case Shape::shape_side:
		//ブロック1
		nextWorldTransform_[0].translate.x = 22.0f;
		nextWorldTransform_[0].translate.y = 8.0f;
		//ブロック2
		nextWorldTransform_[1].translate.x = 22.0f;
		nextWorldTransform_[1].translate.y = 8.0f;
		//ブロック3
		nextWorldTransform_[2].translate.x = 24.0f;
		nextWorldTransform_[2].translate.y = 8.0f;
		//ブロック4
		nextWorldTransform_[3].translate.x = 24.0f;
		nextWorldTransform_[3].translate.y = 8.0f;

	

		break;
	}

	/*-----------------------------------------------------------------------
	
	次の次のブロック

	--------------------------------------------------------------------------*/
	switch (nextShape[2])
	{
	case Shape::shape_I:
		//ブロック1
		scandWorldTransform_[0].translate.x = 22.0f;
		scandWorldTransform_[0].translate.y = -2.0f;
		//ブロック2
		scandWorldTransform_[1].translate.x = 22.0f;
		scandWorldTransform_[1].translate.y = -4.0f;
		//ブロック3
		scandWorldTransform_[2].translate.x = 22.0f;
		scandWorldTransform_[2].translate.y = -6.0f;
		//ブロック4
		scandWorldTransform_[3].translate.x = 22.0f;
		scandWorldTransform_[3].translate.y = -8.0f;

		break;

	case Shape::shape_T:
		//ブロック1
		scandWorldTransform_[0].translate.x = 22.0f;
		scandWorldTransform_[0].translate.y = -2.0f;
		//ブロック2
		scandWorldTransform_[1].translate.x = 20.f;
		scandWorldTransform_[1].translate.y = -2.0f;
		//ブロック3
		scandWorldTransform_[2].translate.x = 24.0f;;
		scandWorldTransform_[2].translate.y = -2.0f;
		//ブロック4
		scandWorldTransform_[3].translate.x = 22.0f;
		scandWorldTransform_[3].translate.y = -4.0f;


		break;

	case Shape::shape_S:
		//ブロック1
		scandWorldTransform_[0].translate.x = 22.0f;
		scandWorldTransform_[0].translate.y = -2.0f;
		//ブロック2
		scandWorldTransform_[1].translate.x = 24.0f;
		scandWorldTransform_[1].translate.y = -2.0f;;
		//ブロック3
		scandWorldTransform_[2].translate.x = 22.0f;
		scandWorldTransform_[2].translate.y = -4.0f;
		//ブロック4
		scandWorldTransform_[3].translate.x = 20.0f;
		scandWorldTransform_[3].translate.y = -4.0f;;


		break;

	case Shape::shape_O:
		//ブロック1
		scandWorldTransform_[0].translate.x = 22.0f;
		scandWorldTransform_[0].translate.y = -2.0f;
		//ブロック2
		scandWorldTransform_[1].translate.x = 24.0f;
		scandWorldTransform_[1].translate.y = -2.0f;
		//ブロック3
		scandWorldTransform_[2].translate.x = 22.0f;
		scandWorldTransform_[2].translate.y = -4.0f;
		//ブロック4
		scandWorldTransform_[3].translate.x = 24.0f;
		scandWorldTransform_[3].translate.y = -4.0f;


		break;

	case Shape::shape_J:
		//ブロック1
		scandWorldTransform_[0].translate.x = 22.0f;
		scandWorldTransform_[0].translate.y = -2.0f;
		//ブロック2
		scandWorldTransform_[1].translate.x = 22.0f;
		scandWorldTransform_[1].translate.y = -4.0f;
		//ブロック3
		scandWorldTransform_[2].translate.x = 22.0f;
		scandWorldTransform_[2].translate.y = -6.0f;
		//ブロック4
		scandWorldTransform_[3].translate.x = 20.0f;
		scandWorldTransform_[3].translate.y = -6.0f;


		break;

	case Shape::shape_L:
		//ブロック1
		scandWorldTransform_[0].translate.x = 22.0f;
		scandWorldTransform_[0].translate.y = -2.0f;
		//ブロック2
		scandWorldTransform_[1].translate.x = 22.0f;
		scandWorldTransform_[1].translate.y = -4.0f;
		//ブロック3
		scandWorldTransform_[2].translate.x = 22.0f;
		scandWorldTransform_[2].translate.y = -6.0f;
		//ブロック4
		scandWorldTransform_[3].translate.x = 24.0f;
		scandWorldTransform_[3].translate.y = -6.0f;


		break;
	case Shape::shape_ten:
		//ブロック1
		scandWorldTransform_[0].translate.x = 22.0f;
		scandWorldTransform_[0].translate.y = -2.0f;
		//ブロック2
		scandWorldTransform_[1].translate.x = 22.0f;
		scandWorldTransform_[1].translate.y = -2.0f;
		//ブロック3
		scandWorldTransform_[2].translate.x = 22.0f;
		scandWorldTransform_[2].translate.y = -2.0f;
		//ブロック4
		scandWorldTransform_[3].translate.x = 22.0f;
		scandWorldTransform_[3].translate.y = -2.0f;
		break;
	case Shape::shape_side:
		//ブロック1
		scandWorldTransform_[0].translate.x = 22.0f;
		scandWorldTransform_[0].translate.y = -2.0f;
		//ブロック2
		scandWorldTransform_[1].translate.x = 22.0f;
		scandWorldTransform_[1].translate.y = -2.0f;
		//ブロック3
		scandWorldTransform_[2].translate.x = 24.0f;
		scandWorldTransform_[2].translate.y = -2.0f;
		//ブロック4
		scandWorldTransform_[3].translate.x = 24.0f;
		scandWorldTransform_[3].translate.y = -2.0f;



		break;
	}
}



void GameObject::Draw(CameraRole viewProjection_){

#ifdef _DEBUG
	ImGui::Begin("WorldTransform");

	if (ImGui::TreeNode("worldTransform_guide")) {
		ImGui::DragFloat3("translate", &worldTransform_guide.translate.x, 0.1f, 100, 100);
		ImGui::DragFloat3("rotate", &worldTransform_guide.rotate.x, 0.1f, 100, 100);
		ImGui::DragFloat3("scale", &worldTransform_guide.scale.x, 0.1f, 100, 100);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("worldTransform_background")) {
		ImGui::DragFloat3("translate", &worldTransform_background.translate.x, 0.1f, 100, 100);
		ImGui::DragFloat3("rotate", &worldTransform_background.rotate.x, 0.1f, 100, 100);
		ImGui::DragFloat3("scale", &worldTransform_background.scale.x, 0.1f, 100, 100);
		ImGui::TreePop();
	}
	ImGui::End();
#endif // RELEASE
	//描画
	Stagemodel_Right->Draw(worldTransform_Right, viewProjection_);
	Stagemodel_Left->Draw(worldTransform_Left, viewProjection_);
	Stagemodel_Bottom->Draw(worldTransform_Bottom, viewProjection_);
	backgroundModel_->Draw(worldTransform_background, viewProjection_);

	guideModel_->Draw(worldTransform_guide, viewProjection_);
	
	for (int i = 0; i < 4; i++) {
		nextModel_[i]->Draw(nextWorldTransform_[i], viewProjection_);

		scandModel_[i]->Draw(scandWorldTransform_[i], viewProjection_);
	}
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
