/// <summary>
/// デッドライン
/// </summary>
#pragma once

#include "Sprite.h"
#include "Triangle.h"
#include "Model.h"
#include "ModelSphere.h"
#include "ModelCube.h"
#include "Input.h"
#include "ImGuiManager/ImGuiManager.h"
#include "BlockCrust.h"
#include "BlockCore.h"
#include "Collider.h"
#include "CollisionConfig.h"
#include "ModelManager.h"
#include "CollisionManager.h"
#include <Object3D.h>
#include <optional>
#include <memory>
#include "Player.h"


class DeadLine {
public:
	///
	///	Default Method
	/// 

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 3Dモデルを使用してラインを描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw3DLine(const CameraRole& viewProjection);


	/// 
	/// User Method
	/// 

	/// Getter
/// Getter
	// ゴールラインに達したかを取得
	bool GetIsDead() { return isDead_; }

	// ワールドトランスフォームを取得
	const WorldTransform& GetWorldTransform() { return worldTransform_; }

	/// Setter
	// 自機のアドレスを取得
	void SetPlayer(Player* player) { player_ = player; }

	// ブロックが消えているかをアドレスで取得
	void SetIsBlockDelete(bool isDelete) { isBlockDelete_ = isDelete; }

private:// プライベートな関数


private:
	// 3Dモデル(デバッグ用)
	std::unique_ptr<Object3DPlacer> model_;
	// ラインに使用するテクスチャ
	uint32_t lineTexture_;



	// 自機
	Player* player_;

	// ワールド座標
	WorldTransform worldTransform_;

	// ブロックが消えたかのフラグ
	bool isBlockDelete_;

	// 自機がデッドラインに達したか
	bool isDead_;

private:// 定数
	// デッドラインの上昇速度
	const float kSpeed_ = 0.002f;
	// デッドラインの下がる大きさ
	const float kDownValue = 1.0f;

	// デッドラインの開始位置
	const float kStartDeadLine_ = -16;


};