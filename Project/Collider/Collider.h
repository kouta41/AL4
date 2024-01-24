#pragma once
#include"Vector3.h"
class Collider {
public:
	//半径を取得
	float GetRadius() const { return radius_; }
	//半径を設定
	void SetRadius(float& Radius) { radius_ = Radius; }
	//衝突時に呼ばれる関数
	virtual void OnCollision() = 0;
	virtual Vector3 GetWorldPosition() = 0;

private:
	/// <summary>
	/// 半径
	/// 初期値は1.0f
	/// </summary>
	float radius_ = 1.0f;
};