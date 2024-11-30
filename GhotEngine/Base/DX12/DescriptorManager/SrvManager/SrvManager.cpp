#include "SrvManager.h"

SrvManager* SrvManager::GetInstance()
{
	static SrvManager instance;
	return &instance;
}

void SrvManager::CreateTextureSrv(Microsoft::WRL::ComPtr<ID3D12Resource> resource, const DirectX::TexMetadata& metadata, uint32_t index)
{
	// metaDataを基にSRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	if (metadata.IsCubemap()) {
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
		srvDesc.TextureCube.MostDetailedMip = 0;
		srvDesc.TextureCube.MipLevels = UINT_MAX;
		srvDesc.TextureCube.ResourceMinLODClamp = 0.0f;
	}
	else {
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; // 2Dテクスチャ
		srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);
	}
	// SRVを作成するDescriptorHeapの場所を決める
	D3D12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV_ = GetDescriptorHeapForCPU(index);

	// SRVの生成
	DirectXCommon::GetInstance()->GetDevice()->CreateShaderResourceView(resource.Get(), &srvDesc, cpuDescHandleSRV_);
}

void SrvManager::CreateInstancingSrv(Resource& resource, uint32_t index)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC instancingSrvDesc{};
	instancingSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	instancingSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	instancingSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	instancingSrvDesc.Buffer.FirstElement = 0;
	instancingSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	instancingSrvDesc.Buffer.NumElements = 100;
	instancingSrvDesc.Buffer.StructureByteStride = sizeof(ParticleForGPU);
	D3D12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV_ = GetDescriptorHeapForCPU(index);
	DirectXCommon::GetInstance()->GetDevice()->CreateShaderResourceView(resource.instancingResource.Get(), &instancingSrvDesc, cpuDescHandleSRV_);
}

void SrvManager::CreatePostProcessSrv(Microsoft::WRL::ComPtr<ID3D12Resource> resource, uint32_t index)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; // 2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;


	D3D12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV_ = GetDescriptorHeapForCPU(index);

	DirectXCommon::GetInstance()->GetDevice()->CreateShaderResourceView(resource.Get(), &srvDesc, cpuDescHandleSRV_);
}

D3D12_CPU_DESCRIPTOR_HANDLE SrvManager::GetDescriptorHeapForCPU(uint32_t index)
{
	return DescriptorManager::GetInstance()->GetCPUDescriptorHandle(DescriptorManager::GetInstance()->GetSRV(), DescriptorManager::GetInstance()->GetDescSize().SRV, index);

}

D3D12_GPU_DESCRIPTOR_HANDLE SrvManager::GetDescriptorHeapForGPU(uint32_t index)
{
	return DescriptorManager::GetInstance()->GetGPUDescriptorHandle(DescriptorManager::GetInstance()->GetSRV(), DescriptorManager::GetInstance()->GetDescSize().SRV, index);
}

void SrvManager::ShiftIndex()
{
	++index_;
}



