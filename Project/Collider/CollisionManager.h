#pragma once
#include "Collider.h"

#include <list>

class CollisionManager{
public: 
	void AddCollider(Collider* collider) { colliders_.push_back(collider); }
	void ClearCollider() { colliders_.clear(); }
	void CheckAllCollisions();

	//geter
	int GethitCount() { return hitCount_; }

	Vector3 GetposA() { return posB; }

private:
	/// <summary>
	/// コライダー
	/// </summary>
	std::list<Collider*> colliders_;

	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);

	int hitCount_ = 0;

	Vector3 posA, posB;
};

