/// <summary>
/// テクスチャの管理
/// </summary>
#pragma once
#include <array>
#include <d3dx12.h>
#include <string>
#include <unordered_map>
#include <wrl.h>
#include <DirectXTex.h>
#include "StringUtility.h"
#include "DirectX12.h"
#include "DescriptorManager/SRVManager/SrvManager.h"
#include <iostream>

class TextureManager {
public:
	/// <summary>
	/// シングルトン
	/// </summary>
	/// <returns></returns>
	static TextureManager* GetInstance();

	/// <summary>
	/// 読み込み
	/// </summary>
	static uint32_t Load(const std::string& fileName);


	// get
	const DirectX::TexMetadata& GetMetaData(uint32_t textureIndex);

private:
	TextureManager() = default;
	~TextureManager() = default;
	TextureManager(const TextureManager&) = delete;
	TextureManager& operator=(const TextureManager&) = delete;

	static DirectX::ScratchImage LoadTexture(const std::string& filePath);

	static void  LoadTexture(const std::string& filePath, uint32_t index);

	static ID3D12Resource* CreateTextureResource(const DirectX::TexMetadata& metadata);

	static void UploadTextureData(Microsoft::WRL::ComPtr<ID3D12Resource> texture, const DirectX::ScratchImage& mipImages);


private: // メンバ変数

	Microsoft::WRL::ComPtr<ID3D12Resource>texResource[128];
	std::unordered_map<std::string, uint32_t> fileHandleMap;
	DirectX::TexMetadata metadata_[128];
};