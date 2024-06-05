#include "CreateResource.h"

// Resource作成
Microsoft::WRL::ComPtr<ID3D12Resource> CreateResource::CreateBufferResource(size_t sizeInBytes) {

	Microsoft::WRL::ComPtr <ID3D12Device> device = DirectXCommon::GetInstance()->GetDevice();

	// 頂点リソース用のヒープの設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD; // UploadHeapを使う
	// 頂点リソースの設定
	D3D12_RESOURCE_DESC vertexResourceDesc{};
	// バッファリソース。テクスチャの場合はまた別の設定をする
	vertexResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	vertexResourceDesc.Width = sizeInBytes; // リソースのサイズ。	
	// バッファの場合はこれらは1にする決まり
	vertexResourceDesc.Height = 1;
	vertexResourceDesc.DepthOrArraySize = 1;
	vertexResourceDesc.MipLevels = 1;
	vertexResourceDesc.SampleDesc.Count = 1;
	// バッファの場合はこれにする決まり
	vertexResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	// 実際に頂点リソースを作る
	Microsoft::WRL::ComPtr<ID3D12Resource> Resource = nullptr;
	HRESULT hr = device.Get()->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE, &vertexResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&Resource));
	assert(SUCCEEDED(hr));

	return Resource;
}

Microsoft::WRL::ComPtr<ID3D12Resource> CreateResource::CreateRenderTextureResource(DXGI_FORMAT format, const Vector4& clearColor){
	Microsoft::WRL::ComPtr <ID3D12Device> device = DirectXCommon::GetInstance()->GetDevice();

	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = 1280; // Textureの幅
	resourceDesc.Height = 720; // Textureの高さ
	resourceDesc.MipLevels = 1; // mipmapの数
	resourceDesc.DepthOrArraySize = 1; // 奥行 or 配列Textureの配列数
	resourceDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // DepthStencilとして使う通知
	resourceDesc.SampleDesc.Count = 1; // サンプリングカウント。1固定
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D; // 2次元
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET; //RenderTargetとして利用可能にする

	// 利用するHeapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT; // VRAM上に作る

	D3D12_CLEAR_VALUE clearValue;
	clearValue.Format = format;
	clearValue.Color[0] = clearColor.x;
	clearValue.Color[1] = clearColor.y;
	clearValue.Color[2] = clearColor.z;
	clearValue.Color[3] = clearColor.w;

	// 実際に頂点リソースを作る
	Microsoft::WRL::ComPtr<ID3D12Resource> Resource = nullptr;
	
	HRESULT hr = device.Get()->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_RENDER_TARGET, &clearValue, IID_PPV_ARGS(&Resource));

	return Resource;
}

// IBV
D3D12_INDEX_BUFFER_VIEW CreateResource::CreateIndexBufferView(Microsoft::WRL::ComPtr<ID3D12Resource> resource, size_t sizeInBytes) {

	// 頂点バッファビューを作成する
	D3D12_INDEX_BUFFER_VIEW IndexBufferView{};
	// リソースの先頭のアドレスから使う
	IndexBufferView.BufferLocation = resource->GetGPUVirtualAddress();
	// 使用するリソースのサイズは頂点3つ分のサイズ
	IndexBufferView.SizeInBytes = uint32_t(sizeInBytes);
	// uint32_tを使う
	IndexBufferView.Format = DXGI_FORMAT_R32_UINT;

	return IndexBufferView;
}

// VBV
D3D12_VERTEX_BUFFER_VIEW CreateResource::CreateVertexBufferView(Microsoft::WRL::ComPtr<ID3D12Resource> resource, size_t sizeInBytes, int size) {


	// 頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	// VertexBufferView
	// リソースの先頭のアドレスから使う
	vertexBufferView.BufferLocation = resource->GetGPUVirtualAddress();
	// 使用するリソースのサイズは頂点3つ分のサイズ
	vertexBufferView.SizeInBytes = UINT(sizeInBytes);
	// 1頂点あたりのサイズ
	vertexBufferView.StrideInBytes = UINT(sizeInBytes / size);

	return vertexBufferView;
}