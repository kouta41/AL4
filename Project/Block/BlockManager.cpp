#include "BlockManager.h"

BlockManager::BlockManager(){
}

BlockManager::~BlockManager(){
	for (BlockCore* core_ : cores_) {
		delete core_;
	}
	for (BlockCrust* crust_ : crusts_) {
		delete crust_;
	}
}

void BlockManager::Initialize(CollisionManager* collisionManager) {
	collisionManager_ = collisionManager;

	worldTransform_.Initialize();
	ModelManager::LoadObjModel("cube.obj");
	
	ModelManager::LoadObjModel("scaffolding.obj");
	coreTexHandle_ = TextureManager::Load("resources/cube.jpg");
	crustTexHandle_= TextureManager::Load("resources/uvChecker.png");
	texHandle_ = TextureManager::Load("resources/white.png");
	texHandle_1= TextureManager::Load("resources/circle.png");

	//パーティクル初期化
	particleSystem= std::make_unique<ParticleSystem>();
	particleSystem->Initialize("plane.obj");
	particleSystem->SetTexHandle(texHandle_1);
	emitter.worldransform.Initialize();
	

	ClearCount_ = 0;
	// 7×7のプレイヤーのデータ

	Block.I = {
		{0,0,1,0,0},
		{0,0,1,0,0},
		{0,0,1,0,0},
		{0,0,1,0,0},
		{0,0,0,0,0},		
	};

	Block.T = {
		{0,0,0,0,0},
		{0,0,0,0,0},
		{0,1,1,1,0},
		{0,0,1,0,0},
		{0,0,0,0,0},
	};

	Block.S = {
		{0,0,0,0,0},
		{0,0,0,0,0},
		{0,0,1,1,0},
		{0,1,1,0,0},
		{0,0,0,0,0},
	};

	Block.O = {
		{0,0,0,0,0},
		{0,0,0,0,0},
		{0,0,1,1,0},
		{0,0,1,1,0},
		{0,0,0,0,0},
	};

	Block.J = {
		{0,0,0,0,0},
		{0,0,1,0,0},
		{0,0,1,0,0},
		{0,1,1,0,0},
		{0,0,0,0,0},
	};

	Block.L = {
		{0,0,0,0,0},
		{0,0,1,0,0},
		{0,0,1,0,0},
		{0,0,1,1,0},
		{0,0,0,0,0},
	};

	Block.Ten = {
		{0,0,0,0,0},
		{0,0,0,0,0},
		{0,0,0,0,0},
		{0,0,1,0,0},
		{0,0,0,0,0},
	};


	Block.Side = {
		{0,0,0,0,0},
		{0,0,0,0,0},
		{0,0,0,0,0},
		{0,0,1,1,0},
		{0,0,0,0,0},
	};



	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();


	for (int i = 0; i < 4; i++) {
		nextWorldTransform_[i].Initialize();
		nextmodel_[i] = std::make_unique<Object3DPlacer>();
		nextmodel_[i]->Initialize();
		nextmodel_[i]->SetModel("cube.obj");
		nextmodel_[i]->SetTexHandle(coreTexHandle_);
		nextmodel_[i]->SetColor({ 1,1,1,0.3f });
		
	}
	worldTransform_.translate = { 0,14,0 };
	

	// 消える判定をとるブロックのx,y座標
	for (int i = 0; i < 15; i++) {
					
		clearBlock_[i].x = (2 * i) - (float)(kBlockNumX-1);
		clearBlock_[i].y = (2 * i) + kMapBottomPos;
	}

	// ブロックが消えるフラグ
	isDelete_ = false;

	//ブロックの形状の設定
	srand((unsigned int)time(nullptr));
	for (int i = 0; i < 3; i++) {
		ChangeShape_[i] = Shape(rand() % 7);
	}

	shape_ = ChangeShape_[0];
	nextShape_ = ChangeShape_[0];

	//クールタイム
	Pushcooltime = 20.f;

	//消えるかの切り替え
	iscollision_ = true;
}

void BlockManager::Update(){
	worldTransform_.UpdateMatrix();

	for (int i = 0; i < 4; i++) {
		nextWorldTransform_[i].UpdateMatrix();
	}
	//移動
	if (input_->PressedKey(DIK_RIGHT)) {
		worldTransform_.translate.x += 2.00f;
	}
	else if (input_->PressedKey(DIK_LEFT)) {
		worldTransform_.translate.x -= 2.00f;
	}

	

	//移動制限
	LimitMove();

	//ブロックの生成
	OutPutBlock();

	//次のブロックの生成
	nextBlockShape();


	for (BlockCore* core_ : cores_) {
		if (iscollision_) {
			core_->Update();
		}
		//クリアラインを超えたら消す（修正必死）
		if (!core_->GetFoolFlag() && core_->GetworldTransform_().y > 10) {
			//core_->SetIsAlive(false);


			core_->SetIsDead(true);
		}


		//デットフラグが立ったら再度設定するコマンド
		if (!core_->GetIsDead()) {
			cores_.remove_if([](BlockCore* core) {
				if (core->GetIsDead()) {
					delete core;
					return true;
				}
				return false;
				});
			// コライダーをすべてクリア
			collisionManager_->ClearColliderList();
			AABB aabb = {
				{-0.99999f,-1.0f,-0.99999f},
				{0.99999f,1.0f,0.99999f}
			};
			// すでに生成されているブロックをコライダーに登録
			// 落下するブロック
			for (BlockCore* core_ : cores_) {
				// 当たり判定の形状を設定
				core_->SetCollisionPrimitive_(kCollisionAABB);
				core_->SetCollisionAttribute_(kAttributeBlock);
				core_->SetAABB_(aabb);
				collisionManager_->SetColliderList(core_);
			}
			// コライダーのすべてが初期化されてしまっているのでplayerを再pushする
			isDelete_ = true;  
		}


	}



	for (BlockCrust* crust_ : crusts_) {
		crust_->Update();
	}


	//デスラグが立つと削除

	


	
	

	crusts_.remove_if([](BlockCrust* crust) {
		if (crust->GetIsDead()) {
			delete crust;
			return true;
		}
		return false;
		});




	if (iscollision_ == false) {
		//横一列になったら消える処理
		OnCollisionLine();
	}
	else {
		for (BlockCore* core_ : cores_) {
			core_->SetIsAlive(true);
		}
	}

	

}

void BlockManager::LimitMove(){

	//移動制限
	if (worldTransform_.translate.x <= -LimitMove_L) {
		worldTransform_.translate.x = -LimitMove_L;
	}
	else if (worldTransform_.translate.x >= LimitMove_R) {
		worldTransform_.translate.x = LimitMove_R;
	}
}

void BlockManager::OutPutBlock(){
	Pushcooltime++;
	if (input_->PressedKey(DIK_SPACE)&& Pushcooltime>=20.f&& iscollision_ == true) {
		Pushcooltime = 0.0f;
		shape_ = ChangeShape_[0];
		nextShape_ = ChangeShape_[1];



		ChangeShape_[0] = ChangeShape_[1];
		ChangeShape_[1] = ChangeShape_[2];
		ChangeShape_[2] = Shape(rand() % 8);

		BlockShape();
	}
	if (input_->PressedKey(DIK_B) && iscollision_ == true) {
		iscollision_ = false;
		iscollisionTime_ = 0.0f;
	}
	if (iscollision_ == false) {
		iscollisionTime_++;
		if (iscollisionTime_ > 90.0f) {
			iscollision_ = true;
		}
	}
}

void BlockManager::BlockShape(){
	switch (shape_)
	{
	case Shape::shape_I:
		playerLocation_ = Block.I;
		break;

	case Shape::shape_T:
		playerLocation_ = Block.T;
		break;

	case Shape::shape_S:
		playerLocation_ = Block.S;
		break;

	case Shape::shape_O:
		playerLocation_ = Block.O;
		break;

	case Shape::shape_J:
		playerLocation_ = Block.J;
		break;

	case Shape::shape_L:
		playerLocation_ = Block.L;
		break;
	case Shape::shape_ten:
		playerLocation_ = Block.Ten;
		break;
	case Shape::shape_side:
		playerLocation_ = Block.Side;
		break;
	}

	for (int i = 0; i < MAX_PLAYER_CHIPS; ++i) {
		for (int j = 0; j < MAX_PLAYER_CHIPS; ++j) {
			if (playerLocation_[i][j] == CORE) {
				BlockCore* newCore = new BlockCore();
				// 初期化
				newCore->Initialize(coreTexHandle_);
				newCore->SetWorldPosition(
					{ worldTransform_.translate.x + (float(j * 2.02f - MAX_PLAYER_CHIPS +0.96f)),
					worldTransform_.translate.y - (float(i * 2.02f - MAX_PLAYER_CHIPS )),
					worldTransform_.translate.z });
				cores_.push_back(newCore);
				collisionManager_->SetColliderList(newCore);
			}
		}
	}
}

void BlockManager::nextBlockShape(){
	
	

	switch (nextShape_)
	{
	case Shape::shape_I:
		//ブロック1
		nextWorldTransform_[0].translate.x = worldTransform_.translate.x;
		nextWorldTransform_[0].translate.y = worldTransform_.translate.y +Shift_y;
		//ブロック2
		nextWorldTransform_[1].translate.x = worldTransform_.translate.x;
		nextWorldTransform_[1].translate.y = worldTransform_.translate.y + Shift_y + Value_y;
		//ブロック3
		nextWorldTransform_[2].translate.x = worldTransform_.translate.x;
		nextWorldTransform_[2].translate.y = worldTransform_.translate.y + Shift_y + Value_y + Value_y;
		//ブロック4
		nextWorldTransform_[3].translate.x = worldTransform_.translate.x;
		nextWorldTransform_[3].translate.y = worldTransform_.translate.y + Shift_y + Value_y + Value_y + Value_y;

		//移動制限
		LimitMove_R = LimitMove_large;
		LimitMove_L = LimitMove_large;

		break;

	case Shape::shape_T:
		//ブロック1
		nextWorldTransform_[0].translate.x = worldTransform_.translate.x;
		nextWorldTransform_[0].translate.y = worldTransform_.translate.y + Shift_y;
		//ブロック2
		nextWorldTransform_[1].translate.x = worldTransform_.translate.x;
		nextWorldTransform_[1].translate.y = worldTransform_.translate.y + Shift_y + Value_y;
		//ブロック3
		nextWorldTransform_[2].translate.x = worldTransform_.translate.x + Shift_x;
		nextWorldTransform_[2].translate.y = worldTransform_.translate.y + Shift_y + Value_y;
		//ブロック4
		nextWorldTransform_[3].translate.x = worldTransform_.translate.x - Shift_x;
		nextWorldTransform_[3].translate.y = worldTransform_.translate.y + Shift_y + Value_y;

		//移動制限
		LimitMove_R = LimitMove_small;
		LimitMove_L = LimitMove_small;

		break;

	case Shape::shape_S:
		//ブロック1
		nextWorldTransform_[0].translate.x = worldTransform_.translate.x;
		nextWorldTransform_[0].translate.y = worldTransform_.translate.y + Shift_y;
		//ブロック2
		nextWorldTransform_[1].translate.x = worldTransform_.translate.x - Shift_x;
		nextWorldTransform_[1].translate.y = worldTransform_.translate.y + Shift_y;
		//ブロック3
		nextWorldTransform_[2].translate.x = worldTransform_.translate.x;
		nextWorldTransform_[2].translate.y = worldTransform_.translate.y + Shift_y + Value_y;
		//ブロック4
		nextWorldTransform_[3].translate.x = worldTransform_.translate.x + Shift_x;
		nextWorldTransform_[3].translate.y = worldTransform_.translate.y + Shift_y + Value_y;

		//移動制限
		LimitMove_R = LimitMove_small;
		LimitMove_L = LimitMove_small;

		break;

	case Shape::shape_O:
		//ブロック1
		nextWorldTransform_[0].translate.x = worldTransform_.translate.x;
		nextWorldTransform_[0].translate.y = worldTransform_.translate.y - 1;
		//ブロック2
		nextWorldTransform_[1].translate.x = worldTransform_.translate.x + 2;
		nextWorldTransform_[1].translate.y = worldTransform_.translate.y - 1;
		//ブロック3
		nextWorldTransform_[2].translate.x = worldTransform_.translate.x;
		nextWorldTransform_[2].translate.y = worldTransform_.translate.y + 1;
		//ブロック4
		nextWorldTransform_[3].translate.x = worldTransform_.translate.x + 2;
		nextWorldTransform_[3].translate.y = worldTransform_.translate.y + 1;

		//移動制限
		LimitMove_R = LimitMove_small;
		LimitMove_L = LimitMove_large;

		break;

	case Shape::shape_J:
		//ブロック1
		nextWorldTransform_[0].translate.x = worldTransform_.translate.x;
		nextWorldTransform_[0].translate.y = worldTransform_.translate.y - 1;
		//ブロック2
		nextWorldTransform_[1].translate.x = worldTransform_.translate.x;
		nextWorldTransform_[1].translate.y = worldTransform_.translate.y + 1;
		//ブロック3
		nextWorldTransform_[2].translate.x = worldTransform_.translate.x;
		nextWorldTransform_[2].translate.y = worldTransform_.translate.y + 3;
		//ブロック4
		nextWorldTransform_[3].translate.x = worldTransform_.translate.x - 2;
		nextWorldTransform_[3].translate.y = worldTransform_.translate.y - 1;

		//移動制限
		LimitMove_R = LimitMove_large;
		LimitMove_L = LimitMove_small;

		break;

	case Shape::shape_L:
		//ブロック1
		nextWorldTransform_[0].translate.x = worldTransform_.translate.x;
		nextWorldTransform_[0].translate.y = worldTransform_.translate.y - 1;
		//ブロック2
		nextWorldTransform_[1].translate.x = worldTransform_.translate.x;
		nextWorldTransform_[1].translate.y = worldTransform_.translate.y + 1;
		//ブロック3
		nextWorldTransform_[2].translate.x = worldTransform_.translate.x;
		nextWorldTransform_[2].translate.y = worldTransform_.translate.y + 3;
		//ブロック4
		nextWorldTransform_[3].translate.x = worldTransform_.translate.x + 2;
		nextWorldTransform_[3].translate.y = worldTransform_.translate.y - 1;

		//移動制限
		LimitMove_R = LimitMove_small;
		LimitMove_L = LimitMove_large;

		break; 
	case Shape::shape_ten:
		//ブロック1
		nextWorldTransform_[0].translate.x = worldTransform_.translate.x;
		nextWorldTransform_[0].translate.y = worldTransform_.translate.y - 1;
		//ブロック2
		nextWorldTransform_[1].translate.x = worldTransform_.translate.x;
		nextWorldTransform_[1].translate.y = worldTransform_.translate.y - 1;
		//ブロック3
		nextWorldTransform_[2].translate.x = worldTransform_.translate.x;
		nextWorldTransform_[2].translate.y = worldTransform_.translate.y - 1;
		//ブロック4
		nextWorldTransform_[3].translate.x = worldTransform_.translate.x;
		nextWorldTransform_[3].translate.y = worldTransform_.translate.y - 1;

		//移動制限
		LimitMove_R = LimitMove_large;
		LimitMove_L = LimitMove_large;

		break;
	case Shape::shape_side:
		//ブロック1
		nextWorldTransform_[0].translate.x = worldTransform_.translate.x;
		nextWorldTransform_[0].translate.y = worldTransform_.translate.y - 1;
		//ブロック2
		nextWorldTransform_[1].translate.x = worldTransform_.translate.x + 2;
		nextWorldTransform_[1].translate.y = worldTransform_.translate.y - 1;
		//ブロック3
		nextWorldTransform_[2].translate.x = worldTransform_.translate.x + 2;
		nextWorldTransform_[2].translate.y = worldTransform_.translate.y - 1;
		//ブロック4
		nextWorldTransform_[3].translate.x = worldTransform_.translate.x;
		nextWorldTransform_[3].translate.y = worldTransform_.translate.y - 1;

		//移動制限
		LimitMove_R = LimitMove_small;
		LimitMove_L = LimitMove_large;

		break;
	}
}



void BlockManager::Draw(const CameraRole& viewProjection_) {


	///デバック場面
#ifdef _DEBUG
	///デバック場面
	ImGui::Begin("worldTransform");
	if (ImGui::TreeNode("worldTransform")) {
		ImGui::DragFloat3("translate", &worldTransform_.translate.x, 0.01f, 100, 100);


		ImGui::TreePop();
	}


	ImGui::End();
	
#endif // _DEBUG

	for (BlockCore* core_ : cores_) {
		core_->Draw(viewProjection_);
	}

	//パーティクル
	if (Particlflag == true) {

		randomEngine = particleSystem->random();
		emitter.worldransform.UpdateMatrix();
		emitter.count = 10; // 一度に生成するパーティクルの数
		emitter.frequency = 1.0f; // 発生頻度
		//emitter.frequencyTime = 0.0f;
		particles = particleSystem->Emission(emitter, randomEngine);

		emitter.frequencyTime += kDeltaTime;
		if (emitter.frequency <= emitter.frequencyTime) {
			particles.splice(particles.end(), particleSystem->Emission(emitter, randomEngine));
			emitter.frequencyTime -= emitter.frequency;
		}

		//particleSystem->Draw(particles, viewProjection_);

		//std::list<Particle> newParticles = particleSystem->Emission(emitter, randomEngine);
		//particles.insert(particles.end(), newParticles.begin(), newParticles.end());

		ParticlTime++;
	}

	if (ParticlTime > 60) {
		Particlflag = false;
	}

	for (BlockCore* core_ : cores_) {
		core_->Draw(viewProjection_);
	}
	for (BlockCrust* crust_ : crusts_) {
		crust_->Draw(viewProjection_);
	}

	for (int i = 0; i < 4; i++) {
		nextmodel_[i]->Draw(nextWorldTransform_[i], viewProjection_);
	}

	

}


void BlockManager::OnCollisionLine(){
	for (int i = 0; i < kBlockNumY; i++) {
		// その列にブロックがいくつあるかの確認
		int count = 0;
		int hardBlockCount = 0;
		// 落下するブロック
		for (BlockCore* core_ : cores_) {
			if (!core_->GetFoolFlag()) {
				if ((int)clearBlock_[i].y == static_cast<int>(std::round(core_->GetWorldPosition().y))) {
					for (int j = 0; j < kBlockNumX; j++) {
						if ((int)clearBlock_[j].x == static_cast<int>(std::round(core_->GetWorldPosition().x))) {
							count++;
							if (!core_->GetIsHardBlock()) {
								core_->SetIsAlive(false);
							}
							else if (core_->GetIsHardBlock()) {
								hardBlockCount++;
							}
						}
					}
				}
			}
		}


		if (count >= kBlockNumX) {
			if (hardBlockCount <= kBlockNumX - 1) {
				Particlflag = true;
				ParticlTime = 0;
				//ClearCount_++;

				//スライドするブロックの設定
				for (BlockCore* core_ : cores_) {
					if (!core_->GetIsAlive()) {
						BlockCrust* newCrust = new BlockCrust();
						// 初期化
						newCrust->Initialize(coreTexHandle_);
						newCrust->SetWorldPosition(core_->GetWorldPosition());
						crusts_.push_back(newCrust);
					}
				}

				cores_.remove_if([](BlockCore* block) {
					if (!block->GetIsAlive()) {
						delete block;
						return true;
					}
					return false;
					});

				// コライダーをすべてクリア
				collisionManager_->ClearColliderList();
				AABB aabb = {
					{-0.99999f,-1.0f,-0.99999f},
					{0.99999f,1.0f,0.99999f}
				};
				// すでに生成されているブロックをコライダーに登録
				// 落下するブロック
				for (BlockCore* core_ : cores_) {
					// 当たり判定の形状を設定
					core_->SetCollisionPrimitive_(kCollisionAABB);
					core_->SetCollisionAttribute_(kAttributeBlock);
					core_->SetAABB_(aabb);
					collisionManager_->SetColliderList(core_);
				}
				// コライダーのすべてが初期化されてしまっているのでplayerを再pushする
				isDelete_ = true;
			}
		}
		else {
			//Particlflag = false;
			
		}
	}
}

void BlockManager::OnCollisonStageOut(){

}

Vector3 BlockManager::GetWorldPosition(){
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld.m[3][0];
	worldPos.y = worldTransform_.matWorld.m[3][1];
	worldPos.z = worldTransform_.matWorld.m[3][2];

	return worldPos;
}

Vector3 BlockManager::GetPlayerCoreWorldPosition()
{
	Vector3 worldPos{};
	for (BlockCore* core_ : cores_) {
		if (core_->GetFoolFlag() == false) {
			worldPos = core_->GetWorldPosition();
		}
	}
	return worldPos;
}

void BlockManager::SetfoolSpeed(float foolspeed){
	for (BlockCore* core_ : cores_) {
		if (core_->GetFoolFlag() == false) {
			core_->SetfoolSpeed(foolspeed);
		}
	}
}

