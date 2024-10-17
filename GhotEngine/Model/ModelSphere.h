/// <summary>
/// モデル球体
/// </summary>
#pragma once
#include <math.h>
#define _USE_MATH_DEFINES
#include "Model.h"
#include "IModelState.h"

class ModelSphere : public IModelState {
public:
	/// <summary>
	///初期化
	/// </summary>
	void Initialize(Model* state)override;
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(WorldTransform worldTransform, CameraRole cameraRole, uint32_t texHandle)override;

private:

	D3D12_VERTEX_BUFFER_VIEW VBV{};
	Resource resource_ = {};
	WorldTransform worldTransform_ = {};
	// 分割数
	uint32_t kSubdivision = 16;

};