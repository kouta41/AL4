#include "Model.h"

Model::~Model()
{
}

/// <summary>
///  obj
/// </summary>
/// <param name="filename"></param>
void Model::InitializeObj(const std::string& filename)
{
	modelData_ = LoadObjFile("resources", filename);

	// VertexResource
	resource_.vertexResource = CreateResource::CreateBufferResource(sizeof(VertexData) * modelData_.vertices.size());
	// VertexBufferView
	// 頂点バッファビューを作成する

	// リソースの先頭のアドレスから使う
	objVertexBufferView_.BufferLocation = resource_.vertexResource->GetGPUVirtualAddress();
	// 使用するリソースのサイズは頂点サイズ
	objVertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * modelData_.vertices.size());
	// 1頂点あたりのサイズ
	objVertexBufferView_.StrideInBytes = sizeof(VertexData);

	// 頂点リソースにデータを書き込む
	VertexData* vertexData = nullptr;
	// 書き込むためのアドレスを取得
	resource_.vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	std::memcpy(vertexData, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size()); // 頂点データをリソースにコピー
	resource_.materialResource = CreateResource::CreateBufferResource(sizeof(Material));
	// データを書き込む
	// アドレスを取得
	resource_.materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->color = { 1.0f,1.0f,1.0f,1.0f };
	materialData_->enableLighting = false;
	materialData_->shininess = 70.0f;

	resource_.cameraResource = CreateResource::CreateBufferResource(sizeof(CameraRole));
	resource_.cameraResource->Map(0, nullptr, reinterpret_cast<void**>(&cameraData_));
	cameraData_->worldPosition = { 0.0f,10.0f,-40.0f };

	resource_.wvpResource = CreateResource::CreateBufferResource(sizeof(TransformationMatrix));

	// 平行光源用のリソース
	resource_.directionalLightResource = CreateResource::CreateBufferResource(sizeof(DirectionalLight));

	// 書き込むためのアドレスを取得
	resource_.directionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
	directionalLightData_->color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData_->direction = Normalize({ 0.0f, -1.0f, 0.0f });
	directionalLightData_->intensity = 1.0f;

	resource_.pointLightResource = CreateResource::CreateBufferResource(sizeof(PointLight));
	resource_.pointLightResource->Map(0, nullptr, reinterpret_cast<void**>(&pointLightData_));
	pointLightData_->color = { 1.0f,1.0f,1.0f,1.0f };
	pointLightData_->position = { 0.0f,10.0f,0.0f };
	pointLightData_->intensity = 1.0f;
	pointLightData_->radius = 12.0f;
	pointLightData_->decay = 0.6f;

	resource_.spotLightResource = CreateResource::CreateBufferResource(sizeof(SpotLight));
	resource_.spotLightResource->Map(0, nullptr, reinterpret_cast<void**>(&spotLightData_));
	spotLightData_->color = { 1.0f,1.0f,1.0f,1.0f };
	spotLightData_->position = { 2.0f,1.25f,0.0f };
	spotLightData_->intensity = 4.0f;
	spotLightData_->direction = Normalize({ -1.0f, -1.0f, 0.0f });
	spotLightData_->distance = 7.0f;
	spotLightData_->cosAngle = std::cos(std::numbers::pi_v<float> / 3.0f);
	spotLightData_->cosFallooffStart = 1.0f;
	spotLightData_->decay = 2.0f;


}

/// <summary>
/// gltf
/// </summary>
/// <param name="filename"></param>
void Model::InitializeGLTF(const std::string& filename)
{
	modelData_ = LoadGLTFFile("resources", filename);

	// VertexResource
	resource_.vertexResource = CreateResource::CreateBufferResource(sizeof(VertexData) * modelData_.vertices.size());
	// VertexBufferView
	// 頂点バッファビューを作成する

	// リソースの先頭のアドレスから使う
	objVertexBufferView_.BufferLocation = resource_.vertexResource->GetGPUVirtualAddress();
	// 使用するリソースのサイズは頂点サイズ
	objVertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * modelData_.vertices.size());
	// 1頂点あたりのサイズ
	objVertexBufferView_.StrideInBytes = sizeof(VertexData);

	// 頂点リソースにデータを書き込む
	VertexData* vertexData = nullptr;
	// 書き込むためのアドレスを取得
	resource_.vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	std::memcpy(vertexData, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size()); // 頂点データをリソースにコピー
	resource_.materialResource = CreateResource::CreateBufferResource(sizeof(Material));
	// データを書き込む
	// アドレスを取得
	resource_.materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->color = { 1.0f,1.0f,1.0f,1.0f };
	materialData_->enableLighting = false;
	materialData_->shininess = 70.0f;

	resource_.cameraResource = CreateResource::CreateBufferResource(sizeof(CameraRole));
	resource_.cameraResource->Map(0, nullptr, reinterpret_cast<void**>(&cameraData_));
	cameraData_->worldPosition = { 0.0f,10.0f,-40.0f };

	resource_.wvpResource = CreateResource::CreateBufferResource(sizeof(TransformationMatrix));

	// 平行光源用のリソース
	resource_.directionalLightResource = CreateResource::CreateBufferResource(sizeof(DirectionalLight));

	// 書き込むためのアドレスを取得
	resource_.directionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
	directionalLightData_->color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData_->direction = Normalize({ 0.0f, -1.0f, 0.0f });
	directionalLightData_->intensity = 1.0f;

	resource_.pointLightResource = CreateResource::CreateBufferResource(sizeof(PointLight));
	resource_.pointLightResource->Map(0, nullptr, reinterpret_cast<void**>(&pointLightData_));
	pointLightData_->color = { 1.0f,1.0f,1.0f,1.0f };
	pointLightData_->position = { 0.0f,10.0f,0.0f };
	pointLightData_->intensity = 1.0f;
	pointLightData_->radius = 12.0f;
	pointLightData_->decay = 0.6f;

	resource_.spotLightResource = CreateResource::CreateBufferResource(sizeof(SpotLight));
	resource_.spotLightResource->Map(0, nullptr, reinterpret_cast<void**>(&spotLightData_));
	spotLightData_->color = { 1.0f,1.0f,1.0f,1.0f };
	spotLightData_->position = { 2.0f,1.25f,0.0f };
	spotLightData_->intensity = 4.0f;
	spotLightData_->direction = Normalize({ -1.0f, -1.0f, 0.0f });
	spotLightData_->distance = 7.0f;
	spotLightData_->cosAngle = std::cos(std::numbers::pi_v<float> / 3.0f);
	spotLightData_->cosFallooffStart = 1.0f;
	spotLightData_->decay = 2.0f;
}

/// <summary>
/// 描画
/// </summary>
/// <param name="worldTransform"></param>
/// <param name="camera"></param>
/// <param name="light"></param>
void Model::Draw(WorldTransform worldTransform, CameraRole cameraRole, Light light)
{

	worldTransform.TransferMatrix(resource_.wvpResource, cameraRole);

	if (light == None) {
		property_ = GraphicsPipeline::GetInstance()->GetPSO().Object3D;
	}
	else if (light == Point) {
		property_ = GraphicsPipeline::GetInstance()->GetPSO().PointLight;
	}
	else if (light == Spot) {
		property_ = GraphicsPipeline::GetInstance()->GetPSO().SpotLight;
	}

	// Rootsignatureを設定。PSOに設定してるけど別途設定が必要
	ID3D12DescriptorHeap* heaps[] = { DescriptorManager::GetInstance()->GetSRV() };
	DirectXCommon::GetCommandList()->SetDescriptorHeaps(_countof(heaps), heaps);
	DirectXCommon::GetCommandList()->SetGraphicsRootSignature(property_.rootSignature_.Get());
	DirectXCommon::GetCommandList()->SetPipelineState(property_.graphicsPipelineState_.Get()); // PSOを設定
	DirectXCommon::GetCommandList()->IASetVertexBuffers(0, 1, &objVertexBufferView_); // VBVを設定
	// 形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	DirectXCommon::GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// マテリアルCBufferの場所を設定
	DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(0, resource_.materialResource->GetGPUVirtualAddress());
	// wvp用のCBufferの場所を設定
	DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(1, resource_.wvpResource->GetGPUVirtualAddress());
	DirectXCommon::GetCommandList()->SetGraphicsRootDescriptorTable(2, SrvManager::GetInstance()->GetGPUHandle(texHandle_));
	// 平行光源
	DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(3, resource_.directionalLightResource->GetGPUVirtualAddress());


	if (light == Point) {
		// カメラ用
		DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(4, resource_.cameraResource->GetGPUVirtualAddress());
		// ポイントライト用
		DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(5, resource_.pointLightResource->GetGPUVirtualAddress());
	}
	else if (light == Spot) {
		// カメラ用
		DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(4, resource_.cameraResource->GetGPUVirtualAddress());
		// ポイントライト用
		DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(5, resource_.spotLightResource->GetGPUVirtualAddress());
	}

	// 描画。(DrawCall/ドローコール)。
	DirectXCommon::GetCommandList()->DrawInstanced(UINT(modelData_.vertices.size()), 1, 0, 0);
}


ModelData Model::LoadObjFile(const std::string& directoryPath, const std::string& filename)
{
	ModelData modelData;
	Assimp::Importer importer;
	std::string filePath = directoryPath + "/" + filename;
	const aiScene* scene = importer.ReadFile(filePath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);
	assert(scene->HasMeshes());

	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		aiMesh* mesh = scene->mMeshes[meshIndex];
		assert(mesh->HasNormals());
		assert(mesh->HasTextureCoords(0));

		for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
			aiFace& face = mesh->mFaces[faceIndex];
			assert(face.mNumIndices == 3);

			for (uint32_t element = 0; element < face.mNumIndices; ++element) {
				uint32_t vertexIndex = face.mIndices[element];
				aiVector3D& position = mesh->mVertices[vertexIndex];
				aiVector3D& normal = mesh->mNormals[vertexIndex];
				aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];
				VertexData vertex;
				vertex.position = { position.x, position.y,position.z,1.0f };
				vertex.normal = { normal.x,normal.y,normal.z };
				vertex.texcoord = { texcoord.x, texcoord.y };
				vertex.position.x *= -1.0f;
				vertex.normal.x *= -1.0f;
				modelData.vertices.push_back(vertex);
			}
		}
	}

	for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; ++materialIndex) {
		aiMaterial* material = scene->mMaterials[materialIndex];
		if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0) {
			aiString textureFilePath;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);
			modelData.material.textureFilePath = directoryPath + "/" + textureFilePath.C_Str();
		}
	}

	return modelData;
}

ModelData Model::LoadGLTFFile(const std::string& directoryPath, const std::string& filename)
{
	ModelData modelData;
	Assimp::Importer importer;
	std::string filePath = directoryPath + "/" + filename;
	const aiScene* scene = importer.ReadFile(filePath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);
	assert(scene->HasMeshes());

	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		aiMesh* mesh = scene->mMeshes[meshIndex];
		assert(mesh->HasNormals());
		assert(mesh->HasTextureCoords(0));

		for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
			aiFace& face = mesh->mFaces[faceIndex];
			assert(face.mNumIndices == 3);

			for (uint32_t element = 0; element < face.mNumIndices; ++element) {
				uint32_t vertexIndex = face.mIndices[element];
				aiVector3D& position = mesh->mVertices[vertexIndex];
				aiVector3D& normal = mesh->mNormals[vertexIndex];
				aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];
				VertexData vertex;
				vertex.position = { position.x, position.y,position.z,1.0f };
				vertex.normal = { normal.x,normal.y,normal.z };
				vertex.texcoord = { texcoord.x, texcoord.y };
				vertex.position.x *= -1.0f;
				vertex.normal.x *= -1.0f;
				modelData.vertices.push_back(vertex);
			}
		}
	}

	for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; ++materialIndex) {
		aiMaterial* material = scene->mMaterials[materialIndex];
		if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0) {
			aiString textureFilePath;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);
			modelData.material.textureFilePath = directoryPath + "/" + textureFilePath.C_Str();
		}
	}

	modelData.rootNode = ReadNode(scene->mRootNode);

	return modelData;
}

MaterialData Model::LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename)
{
	// 変数
	MaterialData materialData; // 構築するmaterialData
	std::string line; // ファイルから読んだ1行を格納するもの
	std::ifstream file(directoryPath + "/" + filename); // ファイルを開く
	assert(file.is_open()); // とりあえず開けなかったら止める
	// ファイルを読み、materialdataを構築
	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;

		// identifierに応じた処理
		if (identifier == "map_Kd") {
			std::string textureFilename;
			s >> textureFilename;
			// 連結してファイルパスにする
			materialData.textureFilePath = directoryPath + "/" + textureFilename;
		}
	}

	return materialData;
}

Node Model::ReadNode(aiNode* node)
{
	Node result;
	aiMatrix4x4 aiLocalMatrix = node->mTransformation; // nodeのlocalMatrixを取得
	aiLocalMatrix.Transpose();
	result.localMatrix.m[0][0] = aiLocalMatrix[0][0];
	result.name = node->mName.C_Str(); // nodeの名前を格納
	result.children.resize(node->mNumChildren); // 子供の数だけ確保
	for (uint32_t childIndex = 0; childIndex < node->mNumChildren; ++childIndex) {
		// 再帰的に読んで階層構造をつくる
		result.children[childIndex] = ReadNode(node->mChildren[childIndex]);
	}

	return result;

}