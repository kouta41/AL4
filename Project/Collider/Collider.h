#pragma once
#include"Vector3.h"

#include "CollisionConfig.h"

class Collider {
public:
	//半径を取得
	float GetRadius() const { return radius_; }
	//半径を設定
	void SetRadius(float& Radius) { radius_ = Radius; }
	//衝突時に呼ばれる関数
	virtual void OnCollision() = 0;
	virtual Vector3 GetWorldPosition() = 0;

	/// <summary>
	/// 衝突属性（自分）を取得
	/// </summary>
	/// <returns></returns>
	uint32_t GetcollisiionAttribute_()const { return collisiionAttribute_; }

	/// <summary>
	/// 衝突属性（自分）を設定
	/// </summary>
	/// <param name="collisiionAttribute"></param>
	void SetcollisiionAttribute_(uint32_t collisiionAttribute) { collisiionAttribute_ = collisiionAttribute; }



	/// <summary>
	/// 衝突属性（相手）を取得
	/// </summary>
	/// <returns></returns>
	uint32_t GetCollisionMask_()const { return CollisionMask_; }

	/// <summary>
	/// 衝突属性（相手）を設定
	/// </summary>
	/// <param name="collisiionAttribute"></param>
	void SetCollisionMask_(uint32_t CollisionMask) { CollisionMask_ = CollisionMask; }

private:
	/// <summary>
	/// 半径
	/// 初期値は1.0f
	/// </summary>
	float radius_ = 1.0f;

	//衝突属性（自分）
	uint32_t collisiionAttribute_ = 0xffffffff;

	//衝突マスク（相手）
	uint32_t CollisionMask_ = 0xffffffff;
};