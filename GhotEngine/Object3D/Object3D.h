#pragma once
#include "ModelManager.h"
#include "WorldTransform.h"
#include "CameraRole.h"

class Object3DPlacer {
public:

	void Initialize();

	void Draw(WorldTransform worldTransform, CameraRole camera);

	// setter
	void SetModel(const std::string& fileName) { model_ = ModelManager::CreateObj(fileName); }

	void SetGLTFModel(const std::string& fileName) { model_ = ModelManager::CreateGLTF(fileName); }

	void SetTexHandle(uint32_t texHandle) { texHandle_ = texHandle; }
	void SetColor(const Vector4& color) { color_ = color; }
//	void SetLight(Lighting* lighting) { lighting_ = lighting; }
	Material SetMaterialProperty(Material materialdata) { return *materialData_ = materialdata; }
	// directionalLightの設定
	DirectionalLight SetLightingProperty(DirectionalLight directionalLight) { return *directionalLightData_ = directionalLight; }


private:
	Model* model_ = nullptr;
	Resource resource_ = {};
	Material* materialData_ = nullptr;
	DirectionalLight* directionalLightData_ = nullptr;
	Property property_{};
	uint32_t texHandle_ = 0;
	Vector4 color_ = {};
//	Lighting* lighting_{};
};