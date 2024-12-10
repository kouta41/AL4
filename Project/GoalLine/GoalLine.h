#pragma once
#include <list>

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

class GoalLine {
public:
	///
	///	Default Method
	/// 

	// コンストラクタ
	GoalLine() = default;
	// デストラクタ
	~GoalLine() = default;

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
	// ゴールラインに達したかを取得
	bool GetIsGoal() { return isGoal_; }

	// ワールド座標を取得(Vector3)
	Vector3 GetWorldPosition() { return worldTransform_.translate; }
	// ワールドトランスフォームを取得
	const WorldTransform& GetWorldTransform() { return worldTransform_; }

	/// Setter
	// ゴールラインに達したかを設定
	void SetIsGoal(bool isGoal) { isGoal_ = isGoal; }

	// 自機のアドレスを取得
	void SetPlayer(Player* player) { player_ = player; }

	// ワールド座標を設定
	void SetWorldPosition(Vector3 pos) { worldTransform_.translate = pos; }

private:// プライベートな関数


private:

	// 3Dモデル(デバッグ用)
	std::unique_ptr<Object3DPlacer> model_;
	// ラインに使用するテクスチャ
	uint32_t lineTexture_;

	// 自機
	Player* player_{};


	// ワールド座標
	WorldTransform worldTransform_;
	// 自機がゴールラインに達したか
	bool isGoal_;
};