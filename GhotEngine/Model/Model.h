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

struct Node {
	Matrix4x4 localMatrix;
	std::string name;
	std::vector<Node> children;
};

struct MaterialData {
	std::string textureFilePath;
};

struct ModelData {
	std::vector<VertexData> vertices;
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
	D3D12_VERTEX_BUFFER_VIEW objVertexBufferView_{};
};