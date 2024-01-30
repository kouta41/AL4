#pragma once
#include <list>

#include "Sprite.h"
#include "Triangle.h"
#include "Model.h"
#include "ModelSphere.h"
#include "ModelCube.h"
#include "Input.h"
#include "ImGuiManager/ImGuiManager.h"
#include "PlayerBullet.h"
#include "Collider.h"
#include "CollisionConfig.h"

/// <summary>
/// プレイヤー
/// </summary>
class Player:public Collider {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	Player();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection viewProjection_);
	void DrawUI(ViewProjection viewProjection_);
	//衝突判定
	void OnCollision();

	//弾リストを取得
	const std::list<PlayerBullet*>& GetPlayerBullsts()const { return bullets_; }
	//ワールド座標系を取得
	Vector3 GetWorldPosition();
	Vector3 GetWorldPosition3DReticle();

	/// <summary>
	/// 親となるワールドトランスフォームをセット
	/// </summary>
	/// <param name="parent"></param>
	void setParent(const WorldTransform* parent);



	void Reticle(const ViewProjection& viewProjection_, const Vector2& pos);

private: // メンバ変数

	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

	//3Dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3DReticle_;

	//3Dモデル
	std::unique_ptr<Model> model_{};
	std::unique_ptr<Model> modelBullet_{};


	//キーボード入力
	Input* input_ = nullptr;

	//弾
	std::list<PlayerBullet*> bullets_;

	//テクスチャハンドル
	uint32_t texHandle_ = 0;
	uint32_t texHandleBullet_ = 0;
	uint32_t textureReticle_ = 0;
	// 2Dレティクル用スプライト
	std::unique_ptr<Sprite> sprite2DReticle_;
};