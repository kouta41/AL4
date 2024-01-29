#pragma once
#include "Collider.h"

#include <list>

class CollisionManager{
public: 
	void AddCollider(Collider* collider) { colliders_.push_back(collider); }
	void ClearCollider() { colliders_.clear(); }
	void CheckAllCollisions();
private:
	/// <summary>
	/// コライダー
	/// </summary>
	std::list<Collider*> colliders_;

	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);
};

