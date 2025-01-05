/// <summary>
/// コライダー
/// </summary>
#pragma once
#include"Vector3.h"
#include "Mathfunction.h"
#include "CollisionConfig.h"

class Collider {
public:

	//半径を取得
	float GetRadius_() const { return radius_; }
	//半径を設定
	void SetRadius_(float& Radius) { radius_ = Radius; }
	//衝突時に呼ばれる関数
	virtual void OnCollision(Collider* collider) {};
	virtual Vector3 GetWorldPosition() = 0;

	/// <summary>
	/// AABBのサイズを取得
	/// </summary>
	/// <returns></returns>
	AABB GetAABB_() { return aabb_; };

	/// <summary>
	/// AABBのサイズを設定
	/// </summary>
	/// <param name="aabb"></param>
	void SetAABB_(AABB& aabb) { aabb_ = aabb; };

	/// <summary>
	/// 衝突属性（自分）を取得
	/// </summary>
	/// <returns></returns>
	uint32_t GetCollisionAttribute_()const { return collisionAttribute_; }

	/// <summary>
	/// 衝突属性（自分）を設定
	/// </summary>
	/// <param name="collisiionAttribute"></param>
	void SetCollisionAttribute_(uint32_t collisionAttribute) { collisionAttribute_ = collisionAttribute; }



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

	/// <summary>
	/// 形状を取得
	/// </summary>
	/// <returns></returns>
	uint32_t GetCollisionPrimitive_() { return collisionPrimitive_; };

	/// <summary>
	/// 形状を設定
	/// </summary>
	/// <param name="collisionPrimitive"></param>
	void SetCollisionPrimitive_(uint32_t collisionPrimitive) { collisionPrimitive_ = collisionPrimitive; };

	/// <summary>
	/// AABBの下面部分に当たったかを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsBottomHitAABB_() { return isBottomHitAABB_; }

	/// <summary>
	/// AABBの下面部分に当たったかを設定
	/// </summary>
	/// <param name="isActive"></param>
	void SetIsBottomHitAABB_(bool isActive) { isBottomHitAABB_ = isActive; }

	/// <summary>
	/// AABBの上面部分に当たったかを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsTopHitAABB_() { return isTopHitAABB_; }

	/// <summary>
	/// AABBの上面部分に当たったかを設定
	/// </summary>
	/// <param name="isActive"></param>
	void SetIsTopHitAABB_(bool isActive) { isTopHitAABB_ = isActive; }

	/// <summary>
	/// AABBの右面部分に当たったかを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsRightHitAABB_() { return isRightHitAABB_; }

	/// <summary>
	/// AABBの右面部分に当たったかを設定
	/// </summary>
	/// <param name="isActive"></param>
	void SetIsRightHitAABB_(bool isActive) { isRightHitAABB_ = isActive; }

	/// <summary>
	/// AABBの左面部分に当たったかを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsLeftHitAABB_() { return isLeftHitAABB_; }

	/// <summary>
	/// AABBの左面部分に当たったかを設定
	/// </summary>
	/// <param name="isActive"></param>
	void SetIsLeftHitAABB_(bool isActive) { isLeftHitAABB_ = isActive; }

	/// <summary>
	/// 解放処理するかを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsDelete_() { return isDelete_; }

	/// <summary>
	/// 解放処理するのかを設定
	/// </summary>
	/// <param name="isActive"></param>
	void SetIsDelete_(bool isActive) { isDelete_ = isActive; }
private:
	/// <summary>
	/// 半径
	/// 初期値は1.0f
	/// </summary>
	float radius_ = 1.0f;

	//AABB
	AABB aabb_{ {-1.0f,-1.0f,-1.0f},{1.0f,1.0f,1.0f} };

	//衝突属性（自分）
	uint32_t collisionAttribute_ = 0xffffffff;

	//衝突マスク（相手）
	uint32_t CollisionMask_ = 0xffffffff;

	//形状(デフォルトは球に設定)
	uint32_t collisionPrimitive_ = kCollisionSphere;

	// 下方向に当たってる
	bool isBottomHitAABB_ = false;
	// 上方向に当たってる
	bool isTopHitAABB_ = false;
	// 右方向に当たってる
	bool isRightHitAABB_ = false;
	// 左方向に当たってる
	bool isLeftHitAABB_ = false;

	// 解放処理をしてもよいか
	bool isDelete_ = false;

};