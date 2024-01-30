#include "GameScene.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	for (Enemy* enemy_ : enemys_) {
		delete enemy_;
	}
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
	player_->setParent(&railCamera_->GetworldTransform_());

	LoadEnemyPopData();
	
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

	UpdateEnemyPopCommands();

	//敵キャラの更新
	for (Enemy* enemy : enemys_) {
		enemy->Update();
	}

	// 弾更新
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Update();
	}
	enemyBullets_.remove_if([](EnemyBullet* bullet_) {
		if (bullet_->IsDead()) {
			delete bullet_;
			return true;
		}
		return false;
		});

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
	//敵キャラの描画
	for (Enemy* enemy : enemys_) {
		enemy->Draw(viewProjection_);
	}

	// 弾描画
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Draw(viewProjection_);
	}
	
}

void GameScene::LoadEnemyPopData()
{
	// ファイルを開く
	std::ifstream file;
	file.open("resources/enemyPop.csv");
	assert(file.is_open());

	// ファイルも内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void GameScene::UpdateEnemyPopCommands()
{
	// 待機処理
	if (isWait) {
		waitTimer--;
		if (waitTimer <= 0) {
			// 待機完了
			isWait = false;
		}
		return;
	}

	// 1行分の文字列を入れる変数
	std::string line;

	// コマンド実行ループ
	while (getline(enemyPopCommands, line)) {
		// 1行分の文字列をストリームに変換して解析しやすくなる
		std::istringstream line_stream(line);

		std::string word;
		// ,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		// "//"から始まる行はコメント
		if (word.find("//") == 0) {
			// コメント行を飛ばす
			continue;
		}

		// POPコマンド
		if (word.find("POP") == 0) {
			// X座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// Y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// Z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			// 敵を発生させる
			EnemyInitialize(Vector3(x, y, z));
		}

		// WAITコマンド
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			// 待ち時間
			int32_t waitTime = atoi(word.c_str());

			// 待機開始
			isWait = true;
			waitTimer = waitTime;

			// コマンドループを抜ける
			break;
		}
	}
}

void GameScene::EnemyInitialize(Vector3)
{
	// 敵キャラの生成
	Enemy* enemy_ = new Enemy();
	// 敵キャラの初期化
	enemy_->Initialize();
	// 敵キャラにゲームシーンを渡す
	enemy_->SetGameScene(this);
	// 敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_.get());
	AddEnemy(enemy_);
}

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet){
	enemyBullets_.push_back(enemyBullet);
}

void GameScene::AddEnemy(Enemy* enemy){
	// リストに登録する
	enemys_.push_back(enemy);
}


void GameScene::CheckAllCollisions(){
	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetPlayerBullsts();
	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemyBullets_;
	// 敵リストの取得
	const std::list<Enemy*>& enemys = enemys_;
	// 自機のコライダーを登録
	collisionManager_->AddCollider(player_.get());
	
	// 自機の弾
	for (PlayerBullet* bullet : playerBullets) {
		collisionManager_->AddCollider(bullet);
	}
	// 敵機の弾
	for (EnemyBullet* bullet : enemyBullets) {
		collisionManager_->AddCollider(bullet);
	}
	// 敵機
	for (Enemy* enemy : enemys) {
		collisionManager_->AddCollider(enemy);
		// 敵弾リストの取得
		const std::list<EnemyBullet*>& enemyBullets = enemy->GetEnemyBullsts();
		for (EnemyBullet* bullet : enemyBullets) {
			collisionManager_->AddCollider(bullet);
		}
	}
	collisionManager_->CheckAllCollisions();
	collisionManager_->ClearCollider();
}