#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <cassert>

class Skydome {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Init(uint32_t texHandle);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection);


private:
	//ワールド変換データ
	WorldTransform worldTransform_;

	//モデル
	std::unique_ptr<Model> model_{};
};