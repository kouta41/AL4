#pragma once
#include "DescriptorManager/DescriptorManager.h"
#include "CreateResource.h"
#define MAX_SRV 128

class SrvManager {
public:

	/// <summary>
	/// シングルトンインスタンス
	/// </summary>
	/// <returns></returns>
	static SrvManager* GetInstance();

	/// <summary>
	/// textureのsrv
	/// </summary>
	/// <param name="resource"></param>
	/// <param name="metadata"></param>
	/// <param name="index"></param>
	void CreateTextureSrv(Microsoft::WRL::ComPtr<ID3D12Resource> resource, const DirectX::TexMetadata& metadata, uint32_t index);

	/// <summary>
	/// instancing用のsrv
	/// </summary>
	/// <param name="resource"></param>
	/// <param name="metadata"></param>
	/// <param name="index"></param>
	void CreateInstancingSrv(Resource& resource,uint32_t index);

	/// <summary>
	/// postProcess用のsrv
	/// </summary>
	/// <param name="resource"></param>
	/// <param name="index"></param>
	void CreatePostProcessSrv(Microsoft::WRL::ComPtr<ID3D12Resource> resource, uint32_t index);

	/// <summary>
	/// DescriptorHeapのCPU用
	/// </summary>
	/// <param name="index"></param>
	/// <returns></returns>
	D3D12_CPU_DESCRIPTOR_HANDLE GetDescriptorHeapForCPU(uint32_t index);

	/// <summary>
	/// DescriptorHeapのGPU用
	/// </summary>
	/// <param name="index"></param>
	/// <returns></returns>
	D3D12_GPU_DESCRIPTOR_HANDLE GetDescriptorHeapForGPU(uint32_t index);

	/// <summary>
	/// srvのgpuhandleの位置をずらす
	/// </summary>
	void ShiftIndex();

#pragma region getter


	uint32_t GetIndex() { return index_; }

#pragma endregion

#pragma region setter

    uint32_t SetIndex(uint32_t index) { return index_ = index; }

#pragma endregion

private:
	SrvManager() = default;
	~SrvManager() = default;
	SrvManager(const SrvManager&) = delete;
	SrvManager& operator=(const SrvManager&) = delete;

private:


	
	uint32_t index_ = 0;

};
