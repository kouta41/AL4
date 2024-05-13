#pragma once

#include "IModelState.h"
#include "ModelSphere.h"
#include "Vector2.h"
#include <fstream>
#include <sstream>
#include "ImGuiManager.h"
#include "CameraRole.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include <numbers>
#include <map>
#include <optional>

struct EulerTransform {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};

struct QuaternionTransform {
	Vector3 scale;
	Quaternion rotate;
	Vector3 translate;
};


struct Joint {
	QuaternionTransform transform;//Transform情報
	Matrix4x4 localMatrix;//localMatrix
	Matrix4x4 skeletonSpaceMatrix;//skeletonSpaceでの変換行列
	std::string name;//名前
	std::vector<int32_t> children;//子jointのIndexのリスト。いなければ空
	int32_t index;//自身のIndex
	std::optional<int32_t> parent;//親のjointのIndex。いなければnull
};

struct Skeleton {
	int32_t root;//RootjointのIndex
	std::map<std::string, int32_t>jointMap;//Joint名とIndexとの辞書
	std::vector<Joint> joints;//所属しているジョイント
};
struct Node {
	QuaternionTransform transform;
	Matrix4x4 localMatrix;
	std::string name;
	std::vector<Node> children;
};

struct MaterialData {
	std::string textureFilePath;
};

struct ModelData {
	std::vector<VertexData> vertices;
	std::vector<uint32_t> indices;
	MaterialData material;
	Node rootNode;
};

class Model {
public:

	~Model();

	/// <summary>
	/// obj初期化
	/// </summary>
	/// <param name="filename"></param>
	void InitializeObj(const std::string& filename);

	/// <summary>
	/// gltf初期化
	/// </summary>
	/// <param name="filename"></param>
	void InitializeGLTF(const std::string& filename);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

#pragma region Setter



#pragma endregion

	/// <summary>
	/// Objファイルを読む
	/// </summary>
	/// <param name="directoryPath"></param>
	/// <param name="filename"></param>
	/// <returns></returns>
	ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// GLTFファイルを読む
	/// </summary>
	/// <param name="directoryPath"></param>
	/// <param name="filename"></param>
	/// <returns></returns>
	ModelData LoadGLTFFile(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// mtlファイルを読む
	/// </summary>
	/// <param name="directoryPath"></param>
	/// <param name="filename"></param>
	/// <returns></returns>
	MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// ノードを読む
	/// </summary>
	/// <param name="node"></param>
	/// <returns></returns>
	Node ReadNode(aiNode* node);

private: // メンバ変数

	ModelData modelData_;
	Resource resource_ = {};
	ID3D12Resource* indexResource_;
	D3D12_VERTEX_BUFFER_VIEW objVertexBufferView_{};
	D3D12_INDEX_BUFFER_VIEW indexBufferViewSprite_{};
};