#pragma once
#include <format>
#include <cassert>

#include "Matrix4x4.h"
#include "Vector3.h"
#include "Vector4.h"
#include<cmath>
#include<numbers>
#include<cassert>
#include <algorithm>


struct DirectionalLight {
	Vector4 color; // ライトの色
	Vector3 direction; // ライトの向き
	float intensity; // 輝度
};

struct AABB {
	Vector3 min; // 最小点
	Vector3 max; // 最大点
};

struct TransForm {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};

struct Emitter {
	TransForm transform;
	uint32_t count;      // 発生数　発生頻度秒に何個出すか
	float frequency;     // 発生頻度
	float frequencyTime; // 頻度用時刻　0で初期化
};

struct Particle_ {
	TransForm transform;
	Vector3 velocity;
	Vector4 color;
	float lifeTime;
	float currentTime;
};

struct AccelerationField {
	Vector3 acceleration;//加速度
	AABB area;//範囲
};

struct ParticleForGPU {
	Matrix4x4 WVP;
	Matrix4x4 World;
	Vector4 color;
};

// ベクトル変換
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

// 拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale);

// X軸周りの回転行列
Matrix4x4 MakeRotateXMatrix(float rotate);

// Y軸周りの回転行列
Matrix4x4 MakeRotateYMatrix(float rotate);

// Z軸周りの回転行列
Matrix4x4 MakeRotateZMatrix(float rotate);

// 回転行列
Matrix4x4 Multiply(Matrix4x4 m1, Matrix4x4 m2);

// 平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

//  アフィン変換行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

//透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

//平行投射行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farCcip);

//逆行(列
Matrix4x4 Inverse(const Matrix4x4& m);

// 単位行列
Matrix4x4 MakeIdentityMatrix();

// 正規化
Vector3 Normalize(const Vector3& v);

//回転行列
Matrix4x4 MakeRotateMatrix(const Vector3& radian);

// 変換
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

//Vector3加算
Vector3 Vector3Add(const Vector3& translate, const Vector3& vector);

//Vector3減算
Vector3 Subtract(const Vector3& m1, const Vector3& m2);

Vector3 Multiply(float scalar, const Vector3& v);

//線形補間
Vector3 VectorLerp(const Vector3& v1, const Vector3& v2, float t);
Vector3 VectorSLerp(const Vector3& v1, const Vector3& v2, float t);
float dot(const Vector3& v1, const Vector3& v2);

Matrix4x4 MakeViewportMatrix(
	float left, float top, float width, float heght, float minDepth, float maxDepth);


/*--------------------演算子オーバーロード---------------------------*/
// 二項演算子
Vector3 operator+(const Vector3& v1, const Vector3& v2);
Vector3 operator+(const Vector3& v1, float s);
Vector3 operator-(const Vector3& v1, const Vector3& v2);
Vector3 operator*(float s, const Vector3& v2);
Vector3 operator*(const Vector3& v, float s);
Vector3 operator/(const Vector3& v, float s);
Matrix4x4 operator+(const Matrix4x4& m1, const Matrix4x4& m2);
Matrix4x4 operator-(const Matrix4x4& m1, const Matrix4x4& m2);
Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2);

// 単項演算子
Vector3 operator-(const Vector3& v);
Vector3 operator+(const Vector3& v);
