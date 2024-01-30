#include "GameScene.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
}

// 初期化
void GameScene::Initialize() {
	worldTransform_.Initialize();
	viewProjection_.Initialize();
	//テクスチャ追加
	texHandleSkydome_ = TextureManager::Load("resources/skydome.jpg");

	
	//レールカメラの生成
	railCamera_ = std::make_unique<RailCamera>();
	//レールカメラの初期化
	railCamera_->Initialize({ 0,0,0 }, { 0,0,0 });

	//自キャラの生成
	player_ = std::make_unique<Player>();
	// 自キャラの初期化
	player_->Initialize();
	//自キャラとレールカメラの親子関係を結ぶ
	player_->setParent(railCamera_->GetworldTransform_());
	//敵の生成
	enemy_ = std::make_unique<Enemy>();
	enemy_->Initialize();

	enemy_->SetPlayer(player_.get());
	
	//天球の生成と初期化
	//3Dモデルの生成
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(texHandleSkydome_);

	collisionManager_ = new CollisionManager();


	
}

// 更新
void GameScene::Update() {

	//レールカメラの更新
	railCamera_->Update();
	CheckAllCollisions();

	//天球の更新
	skydome_->Update();
	//プレイヤーの更新
	player_->Update();

	//敵の更新
	enemy_->Update();

	viewProjection_.matView = railCamera_->GetViewProjection_().matView;
	viewProjection_.matProjection = railCamera_->GetViewProjection_().matProjection;
	viewProjection_.UpdateMatrix();
	worldTransform_.UpdateMatrix();
}

// 描画
void GameScene::Draw() {
	//天球の描画
	skydome_->Draw(viewProjection_);
	//プレイヤーの描画
	player_->Draw(viewProjection_);
	//プレイヤーの描画
	enemy_->Draw(viewProjection_);
}


void GameScene::CheckAllCollisions(){
	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetPlayerBullsts();
	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetEnemyBullsts();
	// 自機のコライダーを登録
	collisionManager_->AddCollider(player_.get());
	// 敵機のコライダーを登録
	collisionManager_->AddCollider(enemy_.get());
	// 自機の弾
	for (PlayerBullet* bullet : playerBullets) {
		collisionManager_->AddCollider(bullet);
	}
	// 敵機の弾
	for (EnemyBullet* bullet : enemyBullets) {
		collisionManager_->AddCollider(bullet);
	}
	collisionManager_->CheckAllCollisions();
	collisionManager_->ClearCollider();
}