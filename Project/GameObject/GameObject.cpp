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
	worldTransform_background1.Initialize();
	worldTransform_guide.Initialize();
	
	//テクスチャのロード
	texHandle_ = TextureManager::Load("resources/1706.png");
	texHandle_UI= TextureManager::Load("resources/8qrp7.png");
	texHandle_chage =TextureManager::Load("resources/rgnk7.png");
	texHandle_ON = TextureManager::Load("resources/5030v.png");
	texHandle_OFF = TextureManager::Load("resources/396iw.png");
	texHandle_Block = TextureManager::Load("resources/grya.png");
	texHandle_background= TextureManager::Load("resources/104696.jpg");
	texHandle_guide = TextureManager::Load("resources/white.png");
	texHandle_moji= TextureManager::Load("resources/20250203_102945.png");
	

	//ステージ右、モデルの初期化
	Stagemodel_Right = std::make_unique<Object3DPlacer>();
	Stagemodel_Right->Initialize();
	Stagemodel_Right->SetModel("cube.obj");
	Stagemodel_Right->SetTexHandle(texHandle_);

	//ステージ左、モデルの初期化
	Stagemodel_Left = std::make_unique<Object3DPlacer>();
	Stagemodel_Left->Initialize();
	Stagemodel_Left->SetModel("cube.obj");
	Stagemodel_Left->SetTexHandle(texHandle_);

	//ステージ下、モデルの初期化
	Stagemodel_Bottom = std::make_unique<Object3DPlacer>();
	Stagemodel_Bottom->Initialize();
	Stagemodel_Bottom->SetModel("cube.obj");
	Stagemodel_Bottom->SetTexHandle(texHandle_);


	//ステージの背景
	backgroundModel_ = std::make_unique<Object3DPlacer>();
	backgroundModel_->Initialize();
	backgroundModel_->SetModel("cube.obj");
	backgroundModel_->SetTexHandle(texHandle_background);

	//ブロックのガイドライン
	guideModel_ = std::make_unique<Object3DPlacer>();
	guideModel_->Initialize();
	guideModel_->SetModel("cube.obj");
	guideModel_->SetTexHandle(texHandle_guide);


	Sprite::StaticInitialize();
	spriteWorldTransform.Initialize();
	spriteWorldTransform_chage.Initialize();
	spriteWorldTransform_ON.Initialize();
	spriteWorldTransform_OFF.Initialize();
	spriteWorldTransform_option.Initialize();

	UIsprite_.reset(Sprite::Create(texHandle_UI));

	UIsprite_chage.reset(Sprite::Create(texHandle_chage));
	UIsprite_chage->SetWorldTransform(spriteWorldTransform_chage);

	UIsprite_ON.reset(Sprite::Create(texHandle_ON));
	UIsprite_ON->SetWorldTransform(spriteWorldTransform_ON);

	UIsprite_OFF.reset(Sprite::Create(texHandle_OFF));
	UIsprite_OFF->SetWorldTransform(spriteWorldTransform_ON);


	UIsprite_option.reset(Sprite::Create(texHandle_moji));
	UIsprite_option->SetWorldTransform(spriteWorldTransform_option);


	for (int i = 0; i < 4; i++) {
		nextWorldTransform_[i].Initialize();
		secondWorldTransform_[i].Initialize();

		nextModel_[i]= std::make_unique<Object3DPlacer>();
		nextModel_[i]->Initialize();
		nextModel_[i]->SetModel("cube.obj");
		nextModel_[i]->SetTexHandle(texHandle_Block);

		scandModel_[i] = std::make_unique<Object3DPlacer>();
		scandModel_[i]->Initialize();
		scandModel_[i]->SetModel("cube.obj");
		scandModel_[i]->SetTexHandle(texHandle_Block);
	}

	//オプション画面背景
	backgroundModel_1 = std::make_unique<Object3DPlacer>();
	backgroundModel_1->Initialize();
	backgroundModel_1->SetModel("cube.obj");
	backgroundModel_1->SetTexHandle(texHandle_guide);
	optionFlag_ = false;


	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

	//調整項目の追加
	globalVariables_ = GlobalVariables::GatInstance();
	//グループを追加
	GlobalVariables::GatInstance()->CreateGroup(groupName);

	////ステージの右側
	//globalVariables_->AddItem(groupName, "Stage_Right_translate", worldTransform_Right.translate);
	//globalVariables_->AddItem(groupName, "Stage_Right_scale", worldTransform_Right.scale);
	////ステージの左側
	//globalVariables_->AddItem(groupName, "Stage_Left_translate", worldTransform_Left.translate);
	//globalVariables_->AddItem(groupName, "Stage_Left_scale", worldTransform_Left.scale);
	////ステージの下側
	//globalVariables_->AddItem(groupName, "Stage_Bottom_translate", worldTransform_Bottom.translate);
	//globalVariables_->AddItem(groupName, "Stage_Bottom_scale", worldTransform_Bottom.scale);
	////ステージの背景
	//globalVariables_->AddItem(groupName, "Stage_background_translate", worldTransform_background.translate);
	//globalVariables_->AddItem(groupName, "Stage_background_scale", worldTransform_background.scale);
	////ステージの背景1
	//globalVariables_->AddItem(groupName, "Stage_background1_translate", worldTransform_background1.translate);
	//globalVariables_->AddItem(groupName, "Stage_background1_scale", worldTransform_background1.scale);
	////スプライトの座標
	//globalVariables_->AddItem(groupName, "Stage_Sprite_translate", spriteWorldTransform_option.translate);
	//globalVariables_->AddItem(groupName, "Stage_Sprite_scale", spriteWorldTransform_option.scale);
	////スプライトUIの座標
	//globalVariables_->AddItem(groupName, "spriteWorldTransform_chage", spriteWorldTransform_chage.translate);
	//globalVariables_->AddItem(groupName, "spriteWorldTransform_ON", spriteWorldTransform_ON.translate);
	//globalVariables_->AddItem(groupName, "spriteWorldTransform_OFF", spriteWorldTransform_OFF.translate);

	

	//ステージのブロックのガイド(スケール)
	//globalVariables_->AddItem(groupName, "Stage_guide_scale_I", worldTransform_guide.scale);
	//globalVariables_->AddItem(groupName, "Stage_guide_scale_T", worldTransform_guide.scale);
	//globalVariables_->AddItem(groupName, "Stage_guide_scale_S", worldTransform_guide.scale);
	//globalVariables_->AddItem(groupName, "Stage_guide_scale_O", worldTransform_guide.scale);
	//globalVariables_->AddItem(groupName, "Stage_guide_scale_J", worldTransform_guide.scale);
	//globalVariables_->AddItem(groupName, "Stage_guide_scale_L", worldTransform_guide.scale);
	//globalVariables_->AddItem(groupName, "Stage_guide_scale_ten", worldTransform_guide.scale);
	//globalVariables_->AddItem(groupName, "Stage_guide_scale_side", worldTransform_guide.scale);
	//ステージのブロックのガイド(座標のオフセット)
	//globalVariables_->AddItem(groupName, "BlockGuide_Offset", BlockGuide_Offset);



	//次のブロックの座標(X軸の三種){X<Y<Z}
	globalVariables_->AddItem(groupName, "NextTranslate_X", NextTranslate_X);
	//次のブロックの座標(Y軸の四種){X<Y<Z<W}
	globalVariables_->AddItem(groupName, "NextTranslate_Y", NextTranslate_Y);

	
	
	//次の次のブロックの座標(X軸の三種){X<Y<Z}
	globalVariables_->AddItem(groupName, "secondTranslate_X", secondTranslate_X);
	//次の次のブロックの座標(Y軸の四種){X<Y<Z<W}
	globalVariables_->AddItem(groupName, "secondTranslate_Y", secondTranslate_Y);

	//他のブロックの角度調整
	globalVariables_->AddItem(groupName, "OtherBlock_rotate", OtherBlock_rotate);
}

void GameObject::Update(){
	//ワールド座標の更新
	worldTransform_Right.UpdateMatrix();
	worldTransform_Left.UpdateMatrix();
	worldTransform_Bottom.UpdateMatrix();
	worldTransform_background.UpdateMatrix();
	worldTransform_background1.UpdateMatrix();
	worldTransform_guide.UpdateMatrix();

	Sprite::StaticUpdate();

	spriteWorldTransform.UpdateMatrix();
	spriteWorldTransform_option.UpdateMatrix();

	UIsprite_option->SetWorldTransform(spriteWorldTransform_option);
	UIsprite_chage->SetWorldTransform(spriteWorldTransform_chage);
	UIsprite_ON->SetWorldTransform(spriteWorldTransform_ON);
	UIsprite_OFF->SetWorldTransform(spriteWorldTransform_ON);

	for (int i = 0; i < 4; i++) {
		nextWorldTransform_[i].rotate = globalVariables_->GetVector3Value(groupName, "OtherBlock_rotate");

		secondWorldTransform_[i].rotate = globalVariables_->GetVector3Value(groupName, "OtherBlock_rotate");
		nextWorldTransform_[i].UpdateMatrix();
		secondWorldTransform_[i].UpdateMatrix();

	}

	//jsonのデータのアップロード
	ApplyGlobalVariaBles();

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
				blockManager_->GetWorldPosition().y - BlockGuide_Offset.y,
				BlockGuide_Offset.z
		};

		worldTransform_guide.scale = globalVariables_->GetVector3Value(groupName, "Stage_guide_scale_I");

		break;

	case Shape::shape_T:
		worldTransform_guide.translate = {
			blockManager_->GetWorldPosition().x,
			blockManager_->GetWorldPosition().y- BlockGuide_Offset.y,
			BlockGuide_Offset.z
		};

		worldTransform_guide.scale = globalVariables_->GetVector3Value(groupName, "Stage_guide_scale_T");

		break;

	case Shape::shape_S:
		worldTransform_guide.translate = {
			blockManager_->GetWorldPosition().x,
			blockManager_->GetWorldPosition().y - BlockGuide_Offset.y,
			BlockGuide_Offset.z
		};

		worldTransform_guide.scale = globalVariables_->GetVector3Value(groupName, "Stage_guide_scale_S");

		break;

	case Shape::shape_O:
		worldTransform_guide.translate = {
				blockManager_->GetWorldPosition().x+ BlockGuide_Offset.x,
				blockManager_->GetWorldPosition().y - BlockGuide_Offset.y,
				BlockGuide_Offset.z
		};

		worldTransform_guide.scale = globalVariables_->GetVector3Value(groupName, "Stage_guide_scale_O");

		break;

	case Shape::shape_J:
		worldTransform_guide.translate = {
					blockManager_->GetWorldPosition().x - BlockGuide_Offset.x,
					blockManager_->GetWorldPosition().y - BlockGuide_Offset.y,
					BlockGuide_Offset.z
		};

		worldTransform_guide.scale = globalVariables_->GetVector3Value(groupName, "Stage_guide_scale_J");
		break;

	case Shape::shape_L:
		worldTransform_guide.translate = {
				blockManager_->GetWorldPosition().x + BlockGuide_Offset.x,
				blockManager_->GetWorldPosition().y - BlockGuide_Offset.y,
				BlockGuide_Offset.z
		};

		worldTransform_guide.scale = globalVariables_->GetVector3Value(groupName, "Stage_guide_scale_L");

		break;
	case Shape::shape_ten:
		worldTransform_guide.translate = {
				blockManager_->GetWorldPosition().x ,
				blockManager_->GetWorldPosition().y - BlockGuide_Offset.y,
				BlockGuide_Offset.z
		};

		worldTransform_guide.scale = globalVariables_->GetVector3Value(groupName, "Stage_guide_scale_ten");

		break;
	case Shape::shape_side:
		worldTransform_guide.translate = {
				blockManager_->GetWorldPosition().x + BlockGuide_Offset.x,
				blockManager_->GetWorldPosition().y - BlockGuide_Offset.y,
				BlockGuide_Offset.z
		};

		worldTransform_guide.scale = globalVariables_->GetVector3Value(groupName, "Stage_guide_scale_side");
		break;
	}


	/*-----------------------------------------------------------------------

	次のブロック

	--------------------------------------------------------------------------*/
	switch (nextShape[1])
	{
	case Shape::shape_I:
		//ブロック1
		nextWorldTransform_[0].translate.x = NextTranslate_X.y;
		nextWorldTransform_[0].translate.y = NextTranslate_Y.w;
		//ブロック2
		nextWorldTransform_[1].translate.x = NextTranslate_X.y;
		nextWorldTransform_[1].translate.y = NextTranslate_Y.z;
		//ブロック3
		nextWorldTransform_[2].translate.x = NextTranslate_X.y;
		nextWorldTransform_[2].translate.y = NextTranslate_Y.y;
		//ブロック4
		nextWorldTransform_[3].translate.x = NextTranslate_X.y;
		nextWorldTransform_[3].translate.y = NextTranslate_Y.x;

		break;

	case Shape::shape_T:
		//ブロック1
		nextWorldTransform_[0].translate.x = NextTranslate_X.y;
		nextWorldTransform_[0].translate.y = NextTranslate_Y.w;
		//ブロック2
		nextWorldTransform_[1].translate.x = NextTranslate_X.x;
		nextWorldTransform_[1].translate.y = NextTranslate_Y.w;
		//ブロック3
		nextWorldTransform_[2].translate.x = NextTranslate_X.z;
		nextWorldTransform_[2].translate.y = NextTranslate_Y.w;
		//ブロック4
		nextWorldTransform_[3].translate.x = NextTranslate_X.y;
		nextWorldTransform_[3].translate.y = NextTranslate_Y.z;


		break;

	case Shape::shape_S:
		//ブロック1
		nextWorldTransform_[0].translate.x = NextTranslate_X.y;
		nextWorldTransform_[0].translate.y = NextTranslate_Y.w;
		//ブロック2
		nextWorldTransform_[1].translate.x = NextTranslate_X.z;
		nextWorldTransform_[1].translate.y = NextTranslate_Y.w;
		//ブロック3
		nextWorldTransform_[2].translate.x = NextTranslate_X.y;
		nextWorldTransform_[2].translate.y = NextTranslate_Y.z;
		//ブロック4
		nextWorldTransform_[3].translate.x = NextTranslate_X.x;
		nextWorldTransform_[3].translate.y = NextTranslate_Y.z;

		
		break;

	case Shape::shape_O:
		//ブロック1
		nextWorldTransform_[0].translate.x = NextTranslate_X.y;
		nextWorldTransform_[0].translate.y = NextTranslate_Y.w;
		//ブロック2
		nextWorldTransform_[1].translate.x = NextTranslate_X.z;
		nextWorldTransform_[1].translate.y = NextTranslate_Y.w;
		//ブロック3
		nextWorldTransform_[2].translate.x = NextTranslate_X.y;
		nextWorldTransform_[2].translate.y = NextTranslate_Y.z;
		//ブロック4
		nextWorldTransform_[3].translate.x = NextTranslate_X.z;
		nextWorldTransform_[3].translate.y = NextTranslate_Y.z;


		break;

	case Shape::shape_J:
		//ブロック1
		nextWorldTransform_[0].translate.x = NextTranslate_X.y;
		nextWorldTransform_[0].translate.y = NextTranslate_Y.w;
		//ブロック2
		nextWorldTransform_[1].translate.x = NextTranslate_X.y;
		nextWorldTransform_[1].translate.y = NextTranslate_Y.z;
		//ブロック3
		nextWorldTransform_[2].translate.x = NextTranslate_X.y;
		nextWorldTransform_[2].translate.y = NextTranslate_Y.y;
		//ブロック4
		nextWorldTransform_[3].translate.x = NextTranslate_X.x;
		nextWorldTransform_[3].translate.y = NextTranslate_Y.y;


		break;

	case Shape::shape_L:
		//ブロック1
		nextWorldTransform_[0].translate.x = NextTranslate_X.y;
		nextWorldTransform_[0].translate.y = NextTranslate_Y.w;
		//ブロック2
		nextWorldTransform_[1].translate.x = NextTranslate_X.y;
		nextWorldTransform_[1].translate.y = NextTranslate_Y.z;
		//ブロック3
		nextWorldTransform_[2].translate.x = NextTranslate_X.y;
		nextWorldTransform_[2].translate.y = NextTranslate_Y.y;
		//ブロック4
		nextWorldTransform_[3].translate.x = NextTranslate_X.z;
		nextWorldTransform_[3].translate.y = NextTranslate_Y.y;


		break;
	case Shape::shape_ten:
		//ブロック1
		nextWorldTransform_[0].translate.x = NextTranslate_X.y;
		nextWorldTransform_[0].translate.y = NextTranslate_Y.w;
		//ブロック2
		nextWorldTransform_[1].translate.x = NextTranslate_X.y;
		nextWorldTransform_[1].translate.y = NextTranslate_Y.w;
		//ブロック3
		nextWorldTransform_[2].translate.x = NextTranslate_X.y;
		nextWorldTransform_[2].translate.y = NextTranslate_Y.w;
		//ブロック4
		nextWorldTransform_[3].translate.x = NextTranslate_X.y;
		nextWorldTransform_[3].translate.y = NextTranslate_Y.w;
		break;
	case Shape::shape_side:
		//ブロック1
		nextWorldTransform_[0].translate.x = NextTranslate_X.y;
		nextWorldTransform_[0].translate.y = NextTranslate_Y.w;
		//ブロック2
		nextWorldTransform_[1].translate.x = NextTranslate_X.y;
		nextWorldTransform_[1].translate.y = NextTranslate_Y.w;
		//ブロック3
		nextWorldTransform_[2].translate.x = NextTranslate_X.z;
		nextWorldTransform_[2].translate.y = NextTranslate_Y.w;
		//ブロック4
		nextWorldTransform_[3].translate.x = NextTranslate_X.z;
		nextWorldTransform_[3].translate.y = NextTranslate_Y.w;

	

		break;
	}

	/*-----------------------------------------------------------------------
	
	次の次のブロック

	--------------------------------------------------------------------------*/
	switch (nextShape[2])
	{
	case Shape::shape_I:
		//ブロック1
		secondWorldTransform_[0].translate.x = secondTranslate_X.y;
		secondWorldTransform_[0].translate.y = secondTranslate_Y.w;
		//ブロック2
		secondWorldTransform_[1].translate.x = secondTranslate_X.y;
		secondWorldTransform_[1].translate.y = secondTranslate_Y.z;
		//ブロック3
		secondWorldTransform_[2].translate.x = secondTranslate_X.y;
		secondWorldTransform_[2].translate.y = secondTranslate_Y.y;
		//ブロック4
		secondWorldTransform_[3].translate.x = secondTranslate_X.y;
		secondWorldTransform_[3].translate.y = secondTranslate_Y.x;

		break;

	case Shape::shape_T:
		//ブロック1
		secondWorldTransform_[0].translate.x = secondTranslate_X.y;
		secondWorldTransform_[0].translate.y = secondTranslate_Y.w;
		//ブロック2
		secondWorldTransform_[1].translate.x = secondTranslate_X.x;
		secondWorldTransform_[1].translate.y = secondTranslate_Y.w;
		//ブロック3
		secondWorldTransform_[2].translate.x = secondTranslate_X.z;
		secondWorldTransform_[2].translate.y = secondTranslate_Y.w;
		//ブロック4
		secondWorldTransform_[3].translate.x = secondTranslate_X.y;
		secondWorldTransform_[3].translate.y = secondTranslate_Y.z;


		break;

	case Shape::shape_S:
		//ブロック1
		secondWorldTransform_[0].translate.x = secondTranslate_X.y;
		secondWorldTransform_[0].translate.y = secondTranslate_Y.w;
		//ブロック2
		secondWorldTransform_[1].translate.x = secondTranslate_X.z;
		secondWorldTransform_[1].translate.y = secondTranslate_Y.w;
		//ブロック3
		secondWorldTransform_[2].translate.x = secondTranslate_X.y;
		secondWorldTransform_[2].translate.y = secondTranslate_Y.z;
		//ブロック4
		secondWorldTransform_[3].translate.x = secondTranslate_X.x;
		secondWorldTransform_[3].translate.y = secondTranslate_Y.z;;


		break;

	case Shape::shape_O:
		//ブロック1
		secondWorldTransform_[0].translate.x = secondTranslate_X.y;
		secondWorldTransform_[0].translate.y = secondTranslate_Y.w;
		//ブロック2
		secondWorldTransform_[1].translate.x = secondTranslate_X.z;
		secondWorldTransform_[1].translate.y = secondTranslate_Y.w;
		//ブロック3
		secondWorldTransform_[2].translate.x = secondTranslate_X.y;
		secondWorldTransform_[2].translate.y = secondTranslate_Y.z;
		//ブロック4
		secondWorldTransform_[3].translate.x = secondTranslate_X.z;
		secondWorldTransform_[3].translate.y = secondTranslate_Y.z;


		break;

	case Shape::shape_J:
		//ブロック1
		secondWorldTransform_[0].translate.x = secondTranslate_X.y;
		secondWorldTransform_[0].translate.y = secondTranslate_Y.w;
		//ブロック2
		secondWorldTransform_[1].translate.x = secondTranslate_X.y;
		secondWorldTransform_[1].translate.y = secondTranslate_Y.z;
		//ブロック3
		secondWorldTransform_[2].translate.x = secondTranslate_X.y;
		secondWorldTransform_[2].translate.y = secondTranslate_Y.y;
		//ブロック4
		secondWorldTransform_[3].translate.x = secondTranslate_X.x;
		secondWorldTransform_[3].translate.y = secondTranslate_Y.y;


		break;

	case Shape::shape_L:
		//ブロック1
		secondWorldTransform_[0].translate.x = secondTranslate_X.y;
		secondWorldTransform_[0].translate.y = secondTranslate_Y.w;
		//ブロック2
		secondWorldTransform_[1].translate.x = secondTranslate_X.y;
		secondWorldTransform_[1].translate.y = secondTranslate_Y.z;
		//ブロック3
		secondWorldTransform_[2].translate.x = secondTranslate_X.y;
		secondWorldTransform_[2].translate.y = secondTranslate_Y.y;
		//ブロック4
		secondWorldTransform_[3].translate.x = secondTranslate_X.z;
		secondWorldTransform_[3].translate.y = secondTranslate_Y.y;


		break;
	case Shape::shape_ten:
		//ブロック1
		secondWorldTransform_[0].translate.x = secondTranslate_X.y;
		secondWorldTransform_[0].translate.y = secondTranslate_Y.w;
		//ブロック2
		secondWorldTransform_[1].translate.x = secondTranslate_X.y;
		secondWorldTransform_[1].translate.y = secondTranslate_Y.w;
		//ブロック3
		secondWorldTransform_[2].translate.x = secondTranslate_X.y;
		secondWorldTransform_[2].translate.y = secondTranslate_Y.w;
		//ブロック4
		secondWorldTransform_[3].translate.x = secondTranslate_X.y;
		secondWorldTransform_[3].translate.y = secondTranslate_Y.w;
		break;
	case Shape::shape_side:
		//ブロック1
		secondWorldTransform_[0].translate.x = secondTranslate_X.y;
		secondWorldTransform_[0].translate.y = secondTranslate_Y.w;
		//ブロック2
		secondWorldTransform_[1].translate.x = secondTranslate_X.y;
		secondWorldTransform_[1].translate.y = secondTranslate_Y.w;
		//ブロック3
		secondWorldTransform_[2].translate.x = secondTranslate_X.z;
		secondWorldTransform_[2].translate.y = secondTranslate_Y.w;
		//ブロック4
		secondWorldTransform_[3].translate.x = secondTranslate_X.z;
		secondWorldTransform_[3].translate.y = secondTranslate_Y.w;



		break;
	}




	if (optionFlag_ == true) {
		worldTransform_background1.translate.y -= 0.2f;
		if (worldTransform_background1.translate.y <= 0) {
			worldTransform_background1.translate.y += 0.2f;

		}
	}
	if(optionFlag_ == false) {
		worldTransform_background1.translate = { 2.0f,2.0f,-83.0f };
	}
}



void GameObject::Draw(CameraRole viewProjection_){


	//描画
	Stagemodel_Right->Draw(worldTransform_Right, viewProjection_);
	Stagemodel_Left->Draw(worldTransform_Left, viewProjection_);
	Stagemodel_Bottom->Draw(worldTransform_Bottom, viewProjection_);
	backgroundModel_->Draw(worldTransform_background, viewProjection_);

	backgroundModel_1->Draw(worldTransform_background1, viewProjection_);

	guideModel_->Draw(worldTransform_guide, viewProjection_);
	
	for (int i = 0; i < 4; i++) {
		nextModel_[i]->Draw(nextWorldTransform_[i], viewProjection_);

		scandModel_[i]->Draw(secondWorldTransform_[i], viewProjection_);
	}


}

void GameObject::Draw2D(CameraRole viewProjection_){
	if (optionFlag_ == false) {
		UIsprite_->Draw();
		UIsprite_chage->Draw();

		if (blockManager_->GetIscollision_()) {
			UIsprite_OFF->Draw();
		}
		else {
			UIsprite_ON->Draw();
		}
	}

	if (optionFlag_ == true) {
		UIsprite_option->Draw();
	}
}

/// <summary>
/// Jsonファイルからデータをアップロード
/// </summary>
void GameObject::ApplyGlobalVariaBles(){

	//ステージの右側
	worldTransform_Right.translate = globalVariables_->GetVector3Value(groupName, "Stage_Right_translate");
	worldTransform_Right.scale = globalVariables_->GetVector3Value(groupName, "Stage_Right_scale");

	//ステージの左側
	worldTransform_Left.translate = globalVariables_->GetVector3Value(groupName, "Stage_Left_translate");
	worldTransform_Left.scale = globalVariables_->GetVector3Value(groupName, "Stage_Left_scale");
	//ステージの下側
	worldTransform_Bottom.translate = globalVariables_->GetVector3Value(groupName, "Stage_Bottom_translate");
	worldTransform_Bottom.scale = globalVariables_->GetVector3Value(groupName, "Stage_Bottom_scale");
	//ステージの背景
	worldTransform_background.translate = globalVariables_->GetVector3Value(groupName, "Stage_background_translate");
	worldTransform_background.scale = globalVariables_->GetVector3Value(groupName, "Stage_background_scale");
	//ステージの背景1
	worldTransform_background1.translate = globalVariables_->GetVector3Value(groupName, "Stage_background1_translate");
	worldTransform_background1.scale = globalVariables_->GetVector3Value(groupName, "Stage_background1_scale");
	//スプライトの座標
	spriteWorldTransform_option.translate = globalVariables_->GetVector3Value(groupName, "Stage_Sprite_translate");
	spriteWorldTransform_option.scale = globalVariables_->GetVector3Value(groupName, "Stage_Sprite_scale");
	//スプライトUIの座標
	spriteWorldTransform_chage.translate= globalVariables_->GetVector3Value(groupName, "spriteWorldTransform_chage");
	spriteWorldTransform_ON.translate = globalVariables_->GetVector3Value(groupName, "spriteWorldTransform_ON");
	spriteWorldTransform_OFF.translate = globalVariables_->GetVector3Value(groupName, "spriteWorldTransform_OFF");
	//ブロックガイドのオフセット
	BlockGuide_Offset = globalVariables_->GetVector3Value(groupName, "BlockGuide_Offset");
	//次のブロックの座標(X軸の三種){X<Y<Z}
	NextTranslate_X=globalVariables_->GetVector3Value(groupName, "NextTranslate_X");
	//次のブロックの座標(Y軸の四種){X<Y<Z<W}
	NextTranslate_Y=globalVariables_->GetVector4Value(groupName, "NextTranslate_Y");
	//次の次のブロックの座標(X軸の三種){X<Y<Z}
	secondTranslate_X = globalVariables_->GetVector3Value(groupName, "secondTranslate_X");
	//次の次のブロックの座標(Y軸の四種){X<Y<Z<W}
	secondTranslate_Y = globalVariables_->GetVector4Value(groupName, "secondTranslate_Y");
	//ブロックの角度
	//OtherBlock_rotate = globalVariables_->GetVector3Value(groupName, "OtherBlock_rotate");
}
