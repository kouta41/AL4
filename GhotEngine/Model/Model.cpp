
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

	resource_.indexResource = CreateResource::CreateBufferResource(sizeof(uint32_t) * modelData_.indices.size());

	// リソースの先頭のアドレスから使う
	indexBufferViewSprite_.BufferLocation = resource_.indexResource->GetGPUVirtualAddress();
	// 使用するリソースのサイズは頂点サイズ
	indexBufferViewSprite_.SizeInBytes = UINT(sizeof(uint32_t) * modelData_.indices.size());
	// 1頂点あたりのサイズ
	indexBufferViewSprite_.Format = DXGI_FORMAT_R32_UINT;

	uint32_t* index = nullptr;
	//// 書き込むためのアドレスを取得
	resource_.indexResource->Map(0, nullptr, reinterpret_cast<void**>(&index));
	std::memcpy(index, modelData_.indices.data(), sizeof(uint32_t) * modelData_.indices.size()); // 頂点データをリソースにコピー;

	// VertexResource
	resource_.vertexResource = CreateResource::CreateBufferResource(sizeof(VertexData) * modelData_.vertices.size());
	//VertexBufferView
	// 頂点バッファビューを作成する

	// リソースの先頭のアドレスから使う
	VertexBufferView_.BufferLocation = resource_.vertexResource->GetGPUVirtualAddress();
	// 使用するリソースのサイズは頂点サイズ
	VertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * modelData_.vertices.size());
	// 1頂点あたりのサイズ
	VertexBufferView_.StrideInBytes = sizeof(VertexData);

	// 頂点リソースにデータを書き込む
	VertexData* vertexData = nullptr;
	// 書き込むためのアドレスを取得
	resource_.vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	std::memcpy(vertexData, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size()); // 頂点データをリソースにコピー;


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
	VertexBufferView_.BufferLocation = resource_.vertexResource->GetGPUVirtualAddress();
	// 使用するリソースのサイズは頂点サイズ
	VertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * modelData_.vertices.size());
	// 1頂点あたりのサイズ
	VertexBufferView_.StrideInBytes = sizeof(VertexData);
	// 頂点リソースにデータを書き込む
	VertexData* vertexData = nullptr;
	// 書き込むためのアドレスを取得
	resource_.vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	std::memcpy(vertexData, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size()); // 頂点データをリソースにコピー

	//IndexResource
	resource_.indexResource = CreateResource::CreateBufferResource(sizeof(uint32_t) * modelData_.indices.size());
	// リソースの先頭のアドレスから使う
	indexBufferViewSprite_.BufferLocation = resource_.indexResource->GetGPUVirtualAddress();
	// 使用するリソースのサイズは頂点サイズ
	indexBufferViewSprite_.SizeInBytes = UINT(sizeof(uint32_t) * modelData_.indices.size());
	// 1頂点あたりのサイズ
	indexBufferViewSprite_.Format = DXGI_FORMAT_R32_UINT;
	uint32_t* index = nullptr;
	//// 書き込むためのアドレスを取得
	resource_.indexResource->Map(0, nullptr, reinterpret_cast<void**>(&index));
	std::memcpy(index, modelData_.indices.data(), sizeof(uint32_t) * modelData_.indices.size()); // 頂点データをリソースにコピー;
}

/// <summary>
/// 描画
/// </summary>
/// <param name="worldTransform"></param>
/// <param name="camera"></param>
/// <param name="light"></param>
void Model::Draw()
{
	DirectXCommon::GetCommandList()->IASetVertexBuffers(0, 1, &VertexBufferView_); // VBVを設定
	DirectXCommon::GetCommandList()->IASetIndexBuffer(&indexBufferViewSprite_); // IBVを設定

	// 描画。(DrawCall/ドローコール)。
	DirectXCommon::GetCommandList()->DrawInstanced(UINT(modelData_.indices.size()), 1, 0, 0);
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
		modelData.vertices.resize(mesh->mNumVertices);//最初に頂点数分のメモリを確保しておく
		for (uint32_t vertexIndex = 0; vertexIndex < mesh->mNumVertices; ++vertexIndex) {
			aiVector3D& position = mesh->mVertices[vertexIndex];
			aiVector3D& normal = mesh->mNormals[vertexIndex];
			aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];
			//右手系ー＞左手系への変換
			modelData.vertices[vertexIndex].position = { -position.x,position.y,position.z,1.0f };
			modelData.vertices[vertexIndex].normal = { -normal.x,normal.y,normal.z };
			modelData.vertices[vertexIndex].texcoord = { texcoord.x,texcoord.y };
		}
		for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
			aiFace& face = mesh->mFaces[faceIndex];
			assert(face.mNumIndices == 3);

			for (uint32_t element = 0; element < face.mNumIndices; ++element) {
				uint32_t vertexIndex = face.mIndices[element];
				modelData.indices.push_back(vertexIndex);
			}
		}

		for (uint32_t boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex) {
			aiBone* bone = mesh->mBones[boneIndex];
			std::string jointName = bone->mName.C_Str();
			JointWeightData& jointWeightData = modelData.skinCluaterData[jointName];

			aiMatrix4x4 bindPoseMatrixAssimp = bone->mOffsetMatrix.Inverse();
			aiVector3D scale, translate;
			aiQuaternion rotate;
			bindPoseMatrixAssimp.Decompose(scale, rotate, translate);
			Matrix4x4 bindPoseMatrix = MakeAffineMatrix(
				{ scale.x,scale.y,scale.z }, { rotate.x,-rotate.y,-rotate.z,rotate.w }, { -translate.x,translate.y,translate.z });
			jointWeightData.inveerseBindPoseMatrix = Inverse(bindPoseMatrix);

			for (uint32_t weightIndex = 0; weightIndex < bone->mNumWeights; ++weightIndex) {
				jointWeightData.vertexWeights.push_back({ bone->mWeights[weightIndex].mWeight,bone->mWeights[weightIndex].mVertexId });
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
		modelData.vertices.resize(mesh->mNumVertices);

		for (uint32_t vertexIndex = 0; vertexIndex < mesh->mNumVertices; ++vertexIndex) {
			aiVector3D& position = mesh->mVertices[vertexIndex];
			aiVector3D& normal = mesh->mNormals[vertexIndex];
			aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];
			//右手系ー＞左手系への変換
			modelData.vertices[vertexIndex].position = { -position.x,position.y,position.z,1.0f };
			modelData.vertices[vertexIndex].normal = { -normal.x,normal.y,normal.z };
			modelData.vertices[vertexIndex].texcoord = { texcoord.x,texcoord.y };
		}
		for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
			aiFace& face = mesh->mFaces[faceIndex];
			assert(face.mNumIndices == 3);

			for (uint32_t element = 0; element < face.mNumIndices; ++element) {
				uint32_t vertexIndex = face.mIndices[element];
				modelData.indices.push_back(vertexIndex);
			}
		}

		for (uint32_t boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex) {
			aiBone* bone = mesh->mBones[boneIndex];
			std::string jointName = bone->mName.C_Str();
			JointWeightData& jointWeightData = modelData.skinCluaterData[jointName];

			aiMatrix4x4 bindPoseMatrixAssimp = bone->mOffsetMatrix.Inverse();
			aiVector3D scale, translate;
			aiQuaternion rotate;
			bindPoseMatrixAssimp.Decompose(scale, rotate, translate);
			Matrix4x4 bindPoseMatrix = MakeAffineMatrix(
				{ scale.x,scale.y,scale.z }, { rotate.x,-rotate.y,-rotate.z,rotate.w }, { -translate.x,translate.y,translate.z });
			jointWeightData.inveerseBindPoseMatrix = Inverse(bindPoseMatrix);

			for (uint32_t weightIndex = 0; weightIndex < bone->mNumWeights; ++weightIndex) {
				jointWeightData.vertexWeights.push_back({ bone->mWeights[weightIndex].mWeight,bone->mWeights[weightIndex].mVertexId });
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
	aiVector3D scale, translate;
	aiQuaternion rotate;
	node->mTransformation.Decompose(scale, rotate, translate);//assimpの行列からSRTを抽出する関数の利用
	result.transform.scale = { scale.x,scale.y,scale.z };//Scaleはそのまま
	result.transform.rotate = { rotate.x,-rotate.y,-rotate.z,rotate.w };//x軸を反転、さらに回転方向が逆なので軸を反転させる
	result.transform.scale = { -translate.x,translate.y,translate.z };//x軸を反転
	result.localMatrix = MakeAffineMatrix(result.transform.scale, result.transform.rotate, result.transform.translate);
	result.name = node->mName.C_Str(); // nodeの名前を格納
	result.children.resize(node->mNumChildren); // 子供の数だけ確保
	for (uint32_t childIndex = 0; childIndex < node->mNumChildren; ++childIndex) {
		// 再帰的に読んで階層構造をつくる
		result.children[childIndex] = ReadNode(node->mChildren[childIndex]);
	}

	return result;

}

SkinCluster Model::CreateSkinCluster(const Microsoft::WRL::ComPtr<ID3D12Device>& device_, const Skeleton& skeleton_, const ModelData& modelData_, const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& descriptorHeap_, uint32_t descriptorSize_){
	SkinCluster skinCluster;
	//palette用のResourceを確保
	skinCluster.paletteResource = CreateResource::CreateBufferResource(sizeof(WeelForGPU) * skeleton_.joints.size());
	WeelForGPU* mappedPalette = nullptr;
	skinCluster.paletteResource->Map(0, nullptr, reinterpret_cast<void**>(&mappedPalette));
	skinCluster.mappedPalette = { mappedPalette,skeleton_.joints.size() };
	skinCluster.paletteSrvHandle.first = DescriptorManager::GetCPUDescriptorHandle(descriptorHeap_.Get(), descriptorSize_, uint32_t( modelData_.indices.size()));
	skinCluster.paletteSrvHandle.second = DescriptorManager::GetGPUDescriptorHandle(descriptorHeap_.Get(), descriptorSize_, uint32_t(modelData_.indices.size()));

	//palette用のsrvを作成。StructuredBufferでアクセスしやすいようにする
	D3D12_SHADER_RESOURCE_VIEW_DESC paletteSrvDesc{};
	paletteSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	paletteSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	paletteSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	paletteSrvDesc.Buffer.FirstElement = 0;
	paletteSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	paletteSrvDesc.Buffer.NumElements = UINT(skeleton_.joints.size());
	paletteSrvDesc.Buffer.StructureByteStride = sizeof(WeelForGPU);
	device_->CreateShaderResourceView(skinCluster.paletteResource.Get(), &paletteSrvDesc, skinCluster.paletteSrvHandle.first);
	
	//influence用のResourceを確保。頂点ごとにinfluence情報を追加できるようにする
	skinCluster.influenceResource = CreateResource::CreateBufferResource(sizeof(VertexInfluence) * modelData_.vertices.size());
	VertexInfluence* mappedInfluence = nullptr;
	skinCluster.influenceResource->Map(0, nullptr, reinterpret_cast<void**>(&mappedInfluence));
	std::memset(mappedInfluence, 0, sizeof(VertexInfluence) * modelData_.vertices.size());//0埋め。weightを0にしておく
	skinCluster.mappedInfluence = { mappedInfluence,modelData_.vertices.size() };

	//Influence用のVBVを作成
	skinCluster.influenceBufferView.BufferLocation = skinCluster.influenceResource->GetGPUVirtualAddress();
	skinCluster.influenceBufferView.SizeInBytes = UINT(sizeof(VertexInfluence) * modelData_.vertices.size());
	skinCluster.influenceBufferView.StrideInBytes = sizeof(VertexInfluence);

	//InveresBindPoseMatrixを格納する場所を作成して、単位行列で埋める
	skinCluster.inverseBindPoseMatrices.resize(skeleton_.joints.size());
	std::generate(skinCluster.inverseBindPoseMatrices.begin(), skinCluster.inverseBindPoseMatrices.end(), MakeIdentityMatrix);

	//ModelDataを解析してInfluenceを埋める
	for (const auto& jointWeight : modelData_.skinCluaterData) {//modelのskinClusterの情報を解析
		auto it = skeleton_.jointMap.find(jointWeight.first);//jointWeight.firstはjoint名なので、skeletonに対象となるjointが含まれているのか判断
		if (it == skeleton_.jointMap.end()) {//そんな名前のjointは存在しない。なので次に回す
			continue;
		}
		//(*it).secondにはjointのindexが入っているので、該当のindexのinverseBindPoseMatrixを代入
		skinCluster.inverseBindPoseMatrices[(*it).second] = jointWeight.second.inveerseBindPoseMatrix;
		for (const auto& vertexWeight : jointWeight.second.vertexWeights) {
			auto& currentInfluence = skinCluster.mappedInfluence[vertexWeight.vertexIndex];//該当のvertexIndexのinfluence情報を参照しておく
			for (uint32_t index = 0; index < kNumMakInfluence; ++index) {//空いているところに入れる
				if (currentInfluence.weights[index] == 0.0f) {//weght==0が空いてる状態なので、その場所にweightとjointのindexを代入
					currentInfluence.weights[index] = vertexWeight.weight;
					currentInfluence.jointIndices[index] = (*it).second;
					break;
				}

			}
		}
	}

	return skinCluster;
}
