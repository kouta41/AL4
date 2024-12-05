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

/// <summary>
/// プレイヤー
/// </summary>
class Player :public Collider{
public://メンバ関数

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Player();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(CollisionManager* collisionManager_);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera_"></param>
	void Draw(const CameraRole& camera_);


	/// <summary>
	/// 当たり判定
	/// </summary>
	/// <param name="collider"></param>
	void OnCollision(Collider* collider)override;




	//ワールド座標系を取得
	Vector3 GetWorldPosition()override;

private://メンバ変数
	WorldTransform worldTransform_;

};

