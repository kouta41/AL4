#include "CollisionManager.h"

void CollisionManager::CheckAllCollisions(){
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		// イテレータAからコライダーを取得
		Collider* colliderA = *itrA;
		// イテレータBはイテレータAの次の要素から回す(重複を避ける)
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;
		for (; itrB != colliders_.end(); ++itrB) {
			Collider* colliderB = *itrB;
			CheckCollisionPair(colliderA, colliderB);
		}
	}
}

void CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB){
	// 判定対象AとBの座標
	posA = colliderA->GetWorldPosition();
	posB = colliderB->GetWorldPosition();
	float Length = (float)sqrt(
		(posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) +
		(posB.z - posA.z) * (posB.z - posA.z));
	// コライダーのフィルターの値でビット演算
	if ((colliderA->GetcollisiionAttribute_() & colliderB->GetCollisionMask_()) == 0 ||
		(colliderB->GetcollisiionAttribute_() & colliderA->GetCollisionMask_()) == 0) {
		return;
	}
	if (Length <= colliderA->GetRadius() + colliderB->GetRadius()) {
		// コライダーAの衝突時コールバック
		colliderA->OnCollision();
		// コライダーBの衝突時コールバック
		colliderB->OnCollision();

		hitCount_++;
	}


}
