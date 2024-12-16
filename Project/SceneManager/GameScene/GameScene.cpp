#include "GameScene.h"
#include <Xinput.h>


#define CUSTOM_DEADZONE 12000


GameScene::GameScene() {}

GameScene::~GameScene() {
	
}

void GameScene::Initialize(){
	worldTransform.Initialize();
	camera.Initialize();
	camera.translate = { 0,0,-85 };

	texHandle_ = TextureManager::Load("resources/white.png");
	SkydometexHandle_ = TextureManager::Load("resources/skydome.jpg");
	blacktexHandle_ = TextureManager::Load("resources/black.png");
	StarttexHandle_ = TextureManager::Load("resources/GO.png");


	ModelManager::LoadObjModel("cube.obj");
	ModelManager::LoadObjModel("skydome.obj");

	// 当たり判定のインスタンスを生成
	collisionManager_ = std::make_unique<CollisionManager>();

	//ブロックの生成
	BlockManager_ = std::make_unique<BlockManager>();
	// ブロックの初期化
	BlockManager_->Initialize(collisionManager_.get());


	player_ = std::make_unique<Player>();
	player_->Initialize();
	collisionManager_->SetColliderList(player_.get());

	// ゴールライン
	goalLine_ = std::make_unique<GoalLine>();
	goalLine_->Initialize();
	goalLine_->SetPlayer(player_.get());
	BlockManager_->SetgoalLine(goalLine_.get());

	// デッドライン
	deadLine_ = std::make_unique<DeadLine>();
	deadLine_->Initialize();
	deadLine_->SetPlayer(player_.get());
	deadLine_->SetIsBlockDelete(BlockManager_->GetIsDelete_());


	//天球の生成
	skydome_ = std::make_unique<Skydome>();
	//天球の初期化
	skydome_->Initialize(SkydometexHandle_);

	//ゲームオブジェクトの生成
	gameObject_ = std::make_unique<GameObject>();
	//ゲームオブジェクトの初期化
	gameObject_->Initialize();

	gameObject_->SetPlayer(BlockManager_.get());

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();


	gameTime = 0;


	worldTransform_1.Initialize();
	worldTransform_2.Initialize();
	worldTransform_3.Initialize();
	worldTransform_4.Initialize();

	
	//モデルの初期化＆設定(エンドオブジェクト右)
	Endrightmodel_ = std::make_unique<Object3DPlacer>();
	Endrightmodel_->Initialize();
	Endrightmodel_->SetModel("cube.obj");
	Endrightmodel_->SetTexHandle(blacktexHandle_);
	EndrightworldTransform_.Initialize();
	EndrightworldTransform_.translate = { 1.0f,-0.0f,camera.translate.z + 4 };
	EndrightworldTransform_.scale = { 1.0f,1.0f,0.0f };

	//モデルの初期化＆設定(エンドオブジェクト左)
	EndLeftmodel_ = std::make_unique<Object3DPlacer>();
	EndLeftmodel_->Initialize();
	EndLeftmodel_->SetModel("cube.obj");
	EndLeftmodel_->SetTexHandle(blacktexHandle_);
	EndLeftworldTransform_.Initialize();
	EndLeftworldTransform_.translate = { -1.0f,-0.0f,camera.translate.z + 4 };
	EndLeftworldTransform_.scale = { 1.0f,1.0f,0.0f };

	//モデルの初期化＆設定(スタートオブジェクト)
	Startmodel_ = std::make_unique<Object3DPlacer>();
	Startmodel_->Initialize();
	Startmodel_->SetModel("cube.obj");
	Startmodel_->SetTexHandle(StarttexHandle_);
	StartworldTransform_.Initialize();
	StartworldTransform_.translate = { -2.0f,-0.87f,-240.0f };
	StartworldTransform_.scale = { 1.0f,1.0f,0.0f };

	flag = true;
	count_ = 0;
	cameraflag1 = true;
	cameraflag2 = true;

	frame1 = 0.0f;
	frame2 = 0.0f;

	endframe1 = 120.0f;
	endframe2 = 60.0f;

	posA = { 0.0f,0.0f,-5.0f };
}

void GameScene::Update(){
	worldTransform.UpdateMatrix();
	worldTransform_1.UpdateMatrix();
	worldTransform_2.UpdateMatrix();
	worldTransform_3.UpdateMatrix();
	worldTransform_4.UpdateMatrix();
	EndrightworldTransform_.UpdateMatrix();
	EndLeftworldTransform_.UpdateMatrix();
	StartworldTransform_.UpdateMatrix();

	camera.UpdateMatrix();

	//ゲームオブジェクトの更新
	gameObject_->Update();

	//天球の更新
	skydome_->Update();

	//自機の更新
	if (BlockManager_->GetIscollision_()) {
		player_->Update();
	}

	//プレイヤーの更新
	BlockManager_->Update();

	// ゴールライン
	goalLine_->Update();

	// デッドライン
	deadLine_->SetIsBlockDelete(BlockManager_->GetisDelete());
//	deadLine_->Update();

	 //ブロックが消えていた場合
	if (BlockManager_->GetisDelete()) {

		AABB aabb = {
			{-0.8f,-1.0f,-0.8f},
			{0.8f,1.0f,0.8f}
		};
		player_->SetAABB_(aabb);
		// 自機をコライダーにセット
		collisionManager_->SetColliderList(player_.get());
		player_->SetCollisionPrimitive_(kCollisionAABB);
		player_->SetCollisionAttribute_(kAttributePlayer);
		// ブロックの消えるフラグをfalse
		BlockManager_->SetIsDelete(false);
	}

	// 当たり判定
	collisionManager_->CheckAllCollisions();

	

	//スタート演出
	if (flag == true) {
		EndrightworldTransform_.translate.x += 0.04f;
		EndLeftworldTransform_.translate.x -= 0.04f;
	}

	if (EndrightworldTransform_.translate.x >= 3.0f) {
			flag = false;
	}

	// 自機が死んだらゲームオーバー
	/*if (!player_->GetIsAlive() && cameraflag1 == true&& cameraflag2 ==true) {
		camera.translate = { posA.x,posA.y ,posA.z-20 };
		cameraRotate = camera.rotate;
		cameraPosA= { posA.x,posA.y ,posA.z - 20 };
		cameraflag1 = false;
	}*/

	if (cameraflag1 == false) {
		frame1++;
		camera.translate.z = cameraPosA.z + (cameraPosA.z - 60 - cameraPosA.z) * easeoutCubic(frame1/ endframe1);
		if (frame1 == endframe1) {
			cameraflag2 = false;
			cameraflag1 = true;
			cameraPosA = camera.translate;
		}
	}

	if (cameraflag2 == false) {
		EndrightworldTransform_.translate.x -= 0.04f;
		EndLeftworldTransform_.translate.x += 0.04f;

		EndrightworldTransform_.translate.z = camera.translate.z+4;
		EndLeftworldTransform_.translate.z = camera.translate.z+4;
		frame2++;
		camera.translate.z=cameraPosA.z+(-0.5f-cameraPosA.z)* easeInQuart(frame2 / endframe2);
		if (frame2 == endframe2) {
			cameraflag2 = true;
			sceneNo_ = END;
		}
	}

	if (input_->PressedKey(DIK_R)) {
		sceneNo_ = TITLE;
	}

	// ゴールラインに達したらクリア
	if (goalLine_->GetIsGoal()) {
		sceneNo_ = END;
	}
	//デッドラインに達したらアウト
	if (deadLine_->GetIsDead()) {
		sceneNo_ = END;
	}

}

void GameScene::Draw(){
#ifdef _DEBUG
	ImGui::Begin("Camera");
	if (ImGui::TreeNode("camera")) {
		ImGui::DragFloat3("translate", &camera.translate.x, 0.1f, 100, 100);
		ImGui::DragFloat3("rotate", &camera.rotate.x, 0.1f, 100, 100);


		ImGui::TreePop();
	}
	if (ImGui::TreeNode("EndrightworldTransform_")) {
		ImGui::DragFloat3("translate", &EndrightworldTransform_.translate.x, 0.1f, 100, 100);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("EndLeftworldTransform_")) {
		ImGui::DragFloat3("translate", &EndLeftworldTransform_.translate.x, 0.1f, 100, 100);
		ImGui::TreePop();
	}
	ImGui::End();
#endif // RELEASE


	//モデルの描画
	//画面遷移
	Endrightmodel_->Draw(EndrightworldTransform_, camera);
	EndLeftmodel_->Draw(EndLeftworldTransform_, camera);

	//天球の描画
	skydome_->Draw(camera);
	//敵の描画
	//enemy_->Draw(camera);
	
	//ゲームオブジェクトの描画
	gameObject_->Draw(camera);
	// ゴールライン
	goalLine_->Draw3DLine(camera);
	// デッドライン
	deadLine_->Draw3DLine(camera);

	if (flag == false) {
		//自機の描画
		player_->Draw(camera);
		
		//プレイヤーの描画
		BlockManager_->Draw(camera);
	}

	gameObject_->Draw2D(camera);

}


