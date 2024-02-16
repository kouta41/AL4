#pragma once
#include "GhotEngin.h"
#include "GraphicsPipeline.h"
#include "StringUtility.h"
#include "TextureManager.h"
#include "CameraRole.h"
#include "WorldTransform.h"
#include "Mathfunction.h"
#include <DirectXTex.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <dxcapi.h>
#include <fstream>
#include <memory>
#include <sstream>
#include <wrl.h>
#include <random>

class Particle {
public: // 静的メンバ変数
	// デスクリプタサイズ
	static UINT sDescriptorHandleIncrementSize_;

	// ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;
	// パイプラインステートオブジェクト
	Microsoft::WRL::ComPtr<ID3D12PipelineState> sPipelineState_;

	Microsoft::WRL::ComPtr<IDxcBlob> vertexShaderBlob_;
	Microsoft::WRL::ComPtr<IDxcBlob> pixelShaderBlob_;

	//BlendMode blendMode_ = BlendMode::kAdd;
	BlendMode blendMode_ = BlendMode::kNone;
	

public:
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns></returns>
	static Particle* GetInstance();

	// 初期化
	void Initialize(const std::string& filename, Emitter emitter);

	// スタート
	void Update();

	void sPipeline();

	/// <summary>
	/// 3Dモデル生成
	/// </summary>
	/// <returns></returns>
	void CreateVertexResource();

	void CreateInstanceSRV();

	void Draw(const ViewProjection& viewProjection);

	static Particle*Create(const std::string& filename, Emitter emitter);

	Particle_ MakeNewParticle(std::mt19937& randomEngine, const TransForm transform);

	std::list<Particle_> Emission(const Emitter& emitter, std::mt19937& randomEngine);

	// パーティクルループ
	void StopParticles();

	void SetTranslate(Vector3 transform) { emitter_.transform.translate = transform; }

	// 色とアルファ値
	void SetColor(Vector4 color);

	// ブレンドモード
	void SetBlendMode(BlendMode blendMode);

	// パーティクル速度
	void SetSpeed(float speed);

	//フィールドをセット
	void SetFiled(AccelerationField accelerationField);

private:
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	Microsoft::WRL::ComPtr<ID3D12Resource> textureResource;
	// 頂点
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_;
	VertexData* vertexData_;

	// WVP用リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> instancingResouce_;
	// マテリアル用リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResorce_;

	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource_;
	uint32_t* indexData_;

	D3D12_CPU_DESCRIPTOR_HANDLE instancingSrvHandelCPU;
	D3D12_GPU_DESCRIPTOR_HANDLE instancingSrvHandelGPU;

	// データを書き込む
	ParticleForGPU* instancingData;
	ModelData modelData;
	Material* materialData = nullptr;

	TextureManager* textureManager_;
	uint32_t texture_;
	uint32_t instancing_;

	uint32_t instanceCount = 1;
	std::list<Particle_> particles;

	Emitter emitter_{};
	AccelerationField accelerationField_;

	float kDeltaTime = 1.0f / 60.0f;
	
	bool loop_ = false;
	bool particle = false;

	Vector4 color_;
	bool isColor = false;

	std::random_device seedGenerator;
	uint32_t descriptorSizeSRV;

private:
	// DirectX::ScratchImage LoadTexture(const std::string& filePath);
	void LoadTexture(const std::string& filename);

	
};