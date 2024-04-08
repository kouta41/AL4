#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"

#include <format>
#include<cmath>
#include<numbers>
#include<cassert>
#include <algorithm>


struct AABB {
	Vector3 min; // 最小点
	Vector3 max; // 最大点
};


// 積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
// 拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale);
// 平行移動
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

/*---------------------------------
 回転行列
------------------------------------*/

// X軸
Matrix4x4 MakeRotateXMatrix(float radian);
// Y軸
Matrix4x4 MakeRotateYMatrix(float radian);
// Z軸
Matrix4x4 MakeRotateZMatrix(float radian);

// アフィン変換
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate);

// ビルボード用のワールド行列作成
Matrix4x4 MakeBiilboardWorldMatrix(const Vector3& scale, const Matrix4x4& billboard, const Vector3& translate);

// 透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

//正射影行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);

//ビューポート行列
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float heght, float minDepth, float maxDepth);

// 逆行列
Matrix4x4 Inverse(const Matrix4x4& m);

// 単位行列
Matrix4x4 MakeIdentityMatrix();

// 正規化
Vector3 Normalize(const Vector3& v);

// 変換
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

//ベクトル変換
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

// ノルム
float Length(const Vector3& v);

// ベクトル減算
Vector3 Subtract(const Vector3& v1, const Vector3& v2);

// スカラー倍
Vector3 Multiply(float scalar, const Vector3& v);

// クロス積
Vector3 Cross(const Vector3& v1, const Vector3& v2);

// 加算
Vector3 Add(const Vector3& v1, const Vector3& v2);

// 減算
Vector3 Subtract(const Vector3& v1, const Vector3& v2);

Vector3 SLerp(const Vector3& v1, const Vector3& v2, float t);

// 内積
float Dot(const Vector3& v1, const Vector3& v2);
