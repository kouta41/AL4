#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"
#include "DirectX12.h"
#include "Mathfunction.h"
#include "CreateResource.h"
#include "CameraRole.h"


//struct TransformationMatrix {
//	Matrix4x4 WVP;
//	Matrix4x4 World;
//	Matrix4x4 WorldInverseTranspose;
//};


struct TransformationMatrix {
	Matrix4x4 WVP; // ローカル → ワールド変換行列
	Matrix4x4 world; // world
	Matrix4x4 WorldInverseTranspose;//worldの逆行列
};


struct WorldTransform {
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> wvpResource;
	// マッピング済みアドレス
	TransformationMatrix* wvpData = nullptr;
	// ローカルスケール
	Vector3 scale = { 1.0f, 1.0f, 1.0f };
	// X,Y,Z軸回りのローカル回転角
	Vector3 rotate = { 0.0f, 0.0f, 0.0f };
	// ローカル座標
	Vector3 translate = { 0.0f, 0.0f, 0.0f };
	// クォータニオン
	Quaternion quaternion = { 0.0f,0.0f,0.0f,1.0f };
	// world
	Matrix4x4 world;
	// ローカル → ワールド変換行列
	Matrix4x4 WVP;

	Matrix4x4 WorldInverseTranspose;
	// 親となるワールド変換へのポインタ
	const WorldTransform* parent = nullptr;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 定数バッファ生成
	/// </summary>
	void CreateConstBuffer();

	/// <summary>
	/// マッピングする
	/// </summary>
	void Map();

	/// <summary>
	/// 行列を転送する
	/// </summary>
	void TransferMatrix();


	/// <summary>
	/// 行列を転送する
	/// </summary>
	void sTransferMatrix(CameraRole camera);


	/// <summary>
	/// ワールド行列を返す
	/// </summary>
	/// <returns></returns>
	Matrix4x4 GetWorldMatrix() const;


	/// <summary>
	/// 行列の更新
	/// </summary>
	void UpdateMatrix();

};