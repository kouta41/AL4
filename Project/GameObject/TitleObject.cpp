#include "TitleObject.h"

TitleObject::TitleObject(){
}

TitleObject::~TitleObject(){
	for (BlockCore* core_ : cores_) {
		delete core_;
	}
}

void TitleObject::Initialize(CollisionManager* collisionManager)
{
	collisionManager_ = collisionManager;
	coreTexHandle_ = TextureManager::Load("resources/grya.png");

}

void TitleObject::Update(){
	//rand関数のリセット
	srand((unsigned int)time(NULL));

	//スライドするブロックの設定

	if (foolTime >= 20) {
		BlockCore* newCore = new BlockCore();
		// 初期化
		newCore->Initialize(coreTexHandle_);
		newCore->SetLandingPosition(LandingPosition_);
		randPos_ = { static_cast<float>(rand() % 21) ,
			12.f,
			static_cast<float>(rand() % 61 - 10)
		};
		randPos_ = { static_cast<float>(rand() % 21) ,
			12.f,
			10.f
		};
		newCore->SetWorldPosition(randPos_);
		cores_.push_back(newCore);
		collisionManager_->SetColliderList(newCore);
		foolTime = 0;
	}
	else {
		foolTime++;
	}

	srand((unsigned int)time(NULL));
	if (foolTime1 >= 30) {
		BlockCore* newCore = new BlockCore();
		// 初期化
		newCore->Initialize(coreTexHandle_);
		newCore->SetLandingPosition(LandingPosition_);
		randPos_ = { static_cast<float>(rand() % 21 - 20) ,
			12.f,
			static_cast<float>(rand() % 41 - 10)
		};
		randPos_ = { static_cast<float>(rand() % 21 - 20) ,
			12.f,
			10.f
		};
		newCore->SetWorldPosition(randPos_);
		cores_.push_back(newCore);
		collisionManager_->SetColliderList(newCore);
		foolTime1 = 0;
	}
	else {
		foolTime1++;
	}


	for (BlockCore* core_ : cores_) {
		core_->Update();
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
		}
		core_->UpdateIskLife();
	}

	//デスラグが立つと削除
	cores_.remove_if([](BlockCore* core) {
		if (core->GetIsDead()) {
			delete core;
			return true;
		}
		return false;
		});
}

void TitleObject::Draw(CameraRole camera){
	//落ちてくるブロック

	for (BlockCore* core_ : cores_) {
		core_->Draw(camera);
	}
}

void TitleObject::Draw2D(CameraRole camera)
{
}

void TitleObject::ApplyGlobalVariaBles()
{
}
