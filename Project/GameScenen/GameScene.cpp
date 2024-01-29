#include "GameScene.h"

GameScene::GameScene() {}

GameScene::~GameScene() {

}

// 初期化
void GameScene::Initialize() {
	worldTransform_.Initialize();
	viewProjection_.Initialize();

	//自キャラの生成
	player_ = std::make_unique<Player>();
	player_->Initialize();

	//敵の生成
	enemy_ = std::make_unique<Enemy>();
	enemy_->Initialize();

	enemy_->SetPlayer(player_.get());
	
}

// 更新
void GameScene::Update() {
	worldTransform_.UpdateMatrix();
	viewProjection_.UpdateMatrix();

	CheckAllcollisons();

	//プレイヤーの更新
	player_->Update();

	//敵の更新
	enemy_->Update();

}

// 描画
void GameScene::Draw() {
	//プレイヤーの描画
	player_->Draw(viewProjection_);
	//プレイヤーの描画
	enemy_->Draw(viewProjection_);
}

void GameScene::CheckAllcollisons()
{	
	// 自弾リストの取得
	const std::list<PlayerBullet*>& PlayerBullets = player_->GetPlayerBullsts();
	// 敵弾リストの取得
	const std::list<EnemyBullet*>& EnemyBullets = enemy_->GetEnemyBullsts();

	//コライダー
	std::list<Collider*> colliders_;
	//コライダーをリストに登録
	colliders_.push_back(player_.get());
	colliders_.push_back(enemy_.get());

	for (PlayerBullet* bullet : PlayerBullets) {
		colliders_.push_back(bullet);
	}
	for (EnemyBullet* bullet : EnemyBullets) {
		colliders_.push_back(bullet);
	}
	//リスト内のペアの総当たり
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		//イテレータAからコライダーを取得
		Collider* colliderA = *itrA;

		//イテレータBはイテレータAの次の要素から回す
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;
		for (; itrB != colliders_.end(); ++itrB) {
			Collider* colliderB = *itrB;
			CheckCollisionPair(colliderA, colliderB);
		}
	}
}

void GameScene::CheckCollisionPair(Collider* colliderA, Collider* colliderB){
	// 判定対象AとBの座標
	Vector3 posA, posB;
	posA = colliderA->GetWorldPosition();
	posB = colliderB->GetWorldPosition();
	float Length = sqrt(
		(posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) +
		(posB.z - posA.z) * (posB.z - posA.z));
	//コライダーのフィルターの値でビット演算
	if ((colliderA->GetcollisiionAttribute_() & colliderB->GetCollisionMask_()) == 0 ||
		(colliderB->GetcollisiionAttribute_() & colliderA->GetCollisionMask_() )== 0) {
		return;
	}
	if (Length <= colliderA->GetRadius() + colliderB->GetRadius()) {
		//コライダーAの衝突時コールバック
		colliderA->OnCollision();
		//コライダーBの衝突時コールバック
		colliderB->OnCollision();
	}
}
