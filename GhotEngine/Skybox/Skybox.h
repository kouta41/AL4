/// <summary>
/// スカイボックス
/// </summary>
#pragma once
#include "WorldTransform.h"
#include "GraphicsPipeline.h"
#include "TextureManager.h"
#include "IModelState.h"

class Skybox  {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="state"></param>
	void Initialize();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="worldTransform"></param>
	/// <param name="viewProjection"></param>
	/// <param name="texHandle"></param>
	void Draw(WorldTransform worldTransform, CameraRole cameraRole, uint32_t texHandle);

private:

	D3D12_VERTEX_BUFFER_VIEW VBV{};
	Resource resource_ = {};
	WorldTransform worldTransform_ = {};
};

