//#include "Particle.h"
//#include "ImGuiManager.h"
//#include <cassert>
//#include <format>
//#include <numbers>
//
//bool IsCollision(const AABB& aabb, const Vector3& point) {
//	if ((aabb.min.x <= point.x && point.x <= aabb.max.x) &&
//	    (aabb.min.y <= point.y && point.y <= aabb.max.y) &&
//	    (aabb.min.z <= point.z && point.z <= aabb.max.z)) {
//		return true;
//	}
//
//	return false;
//}
//
//void Particle::Initialize(const std::string& filename, Emitter emitter) {
//	emitter_ = emitter;
//	instanceCount = emitter_.count;
//	accelerationField_.acceleration = {0.0f, 0.0f, 0.0f};
//	LoadTexture(filename);
//	CreateVertexResource();
//	sPipeline();
//	CreateInstanceSRV();
//}
//
//void Particle::sPipeline() {
//	vertexShaderBlob_ = GraphicsPipeline::GetInstance()->CreateParticleVSShader();
//	pixelShaderBlob_ = GraphicsPipeline::GetInstance()->CreateParticlePSShader();
//
//	rootSignature_ = GraphicsPipeline::GetInstance()->CreateParticleRootSignature();
//	sPipelineState_ = GraphicsPipeline::GetInstance()->CreateParticleGraphicsPipeline(blendMode_);
//};
//
//void Particle::Update() { particle = true; }
//
//void Particle::StopParticles() { particle = false; }
//
//void Particle::Draw(const ViewProjection& viewProjection) {
//	// 乱数生成
//	std::mt19937 randomEngine(seedGenerator());
//	Matrix4x4 backToFrontMatrix = MakeRotateYMatrix(std::numbers::pi_v<float>);
//	Matrix4x4 cameraMatrix = MakeAffineMatrix(
//	    {1.0f, 1.0f, 1.0f}, viewProjection.rotate, viewProjection.translate);
//	Matrix4x4 billboardMatrix = backToFrontMatrix * cameraMatrix;
//	billboardMatrix.m[3][0] = 0.0f;
//	billboardMatrix.m[3][1] = 0.0f;
//	billboardMatrix.m[3][2] = 0.0f;
//
//	uint32_t numInstance = 0;
//
//	emitter_.frequencyTime += kDeltaTime;
//	if (particle) {
//		if (emitter_.frequency <= emitter_.frequencyTime) {
//			particles.splice(particles.end(), Emission(emitter_, randomEngine));
//			emitter_.frequencyTime -= emitter_.frequency;
//		}
//	}
//
//	for (std::list<Particle_>::iterator particleIterator = particles.begin();
//	     particleIterator != particles.end();) {
//
//		if ((*particleIterator).lifeTime <= (*particleIterator).currentTime) {
//			particleIterator = particles.erase(particleIterator);
//			continue;
//		}
//
//		if (IsCollision(accelerationField_.area, (*particleIterator).transform.translate)) {
//			(*particleIterator).velocity += accelerationField_.acceleration * kDeltaTime;
//		}
//
//		(*particleIterator).transform.translate += (*particleIterator).velocity * kDeltaTime;
//		(*particleIterator).currentTime += kDeltaTime;
//
//		if (numInstance < instanceCount) {
//			Matrix4x4 worldMatrix =
//			    MakeScaleMatrix((*particleIterator).transform.scale) * billboardMatrix *
//			    MakeTranslateMatrix((*particleIterator).transform.translate);
//
//			instancingData[numInstance].World = worldMatrix;
//
//			float alph = 1.0f - ((*particleIterator).currentTime / (*particleIterator).lifeTime);
//			instancingData[numInstance].color = (*particleIterator).color;
//			instancingData[numInstance].color.w = alph;
//			numInstance++;
//		}
//
//		++particleIterator;
//	}
//
//	// RootSignatureを設定。PSOに設定しているけど別途設定が必要
//	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootSignature(rootSignature_.Get());
//	DirectXCommon::GetInstance()->GetCommandList()->SetPipelineState(sPipelineState_.Get());
//
//	// 形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
//	DirectXCommon::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//	DirectXCommon::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);
//
//	DirectXCommon::GetInstance()->GetCommandList()->SetDescriptorHeaps(1, Engine::GetSRV().GetAddressOf());
//	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(1, textureManager_->GetParticleGPUHandle(instancing_));
//	//Engine::GetList()->SetGraphicsRootDescriptorTable(1, instancingSrvHandelGPU);
//
//
//	// wvp用のCBufferの場所を設定
//	// マテリアルCBufferの場所を設定
//	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(
//	    0, materialResorce_->GetGPUVirtualAddress());
//	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(
//	    4, viewProjection.constBuff_->GetGPUVirtualAddress());
//	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(2, textureManager_->GetGPUHandle(texture_));
//
//	// 三角形の描画
//	DirectXCommon::GetInstance()->GetCommandList()->DrawInstanced(UINT(modelData.vertices.size()), numInstance, 0, 0);
//	numInstance = 0;
//}
//
//// 頂点データの設定
//void Particle::CreateVertexResource() {
//	instancingResouce_ = CreateResource::CreateBufferResource( sizeof(ParticleForGPU) * instanceCount);
//	// データを書き込む
//	instancingData = nullptr;
//	// 書き込むためのアドレスを取得
//	instancingResouce_->Map(0, nullptr, reinterpret_cast<void**>(&instancingData));
//	// 単位行列を書き込む
//	for (uint32_t i = 0; i < instanceCount; ++i) {
//		instancingData[i].World = MakeIdentity4x4();
//		instancingData[i].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
//	}
//
//	// 頂点リソースを作る
//	vertexResource_ = CreateResource::CreateBufferResource																						( sizeof(VertexData) * modelData.vertices.size());
//	vertexResource_->Map(
//	    0, nullptr, reinterpret_cast<void**>(&vertexData_)); // 書き込むためのアドレスを取得
//
//	// 頂点バッファビューを作成する
//	vertexBufferView.BufferLocation =
//	    vertexResource_->GetGPUVirtualAddress(); // リソースの先頭のアドレスから使う
//	vertexBufferView.SizeInBytes = UINT(
//	    sizeof(VertexData) * modelData.vertices.size()); // 使用するリソースのサイズは頂点サイズ
//	vertexBufferView.StrideInBytes = sizeof(VertexData); // 1頂点あたりのサイズ
//
//	std::memcpy(
//	    vertexData_, modelData.vertices.data(),
//	    sizeof(VertexData) * modelData.vertices.size()); // 頂点データをリソースにコピース
//
//	// マテリアル
//	materialResorce_ = CreateBufferResoure( sizeof(Material));
//
//	// マテリアルにデータを書き込む
//	// 書き込むためのアドレスを取得
//	materialResorce_->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
//	// 今回は白を書き込む
//	materialData->color.rgb = Vector3(1.0f, 1.0f, 1.0f);
//	materialData->color.a = float(1.0f);
//	// Lightingを有効にする
//	materialData->enableLighting = false;
//	// 初期化
//	materialData->uvTransform = Math::MakeIdentity4x4();
//};
//
//void Particle::SetSpeed(float speed) { kDeltaTime = speed / 60.0f; }
//
//void Particle::SetColor(Vector4 color) {
//	color_ = color;
//	isColor = true;
//}
//
//void Particle::SetBlendMode(BlendMode blendMode) { blendMode_ = blendMode; }
//
//void Particle::SetFiled(AccelerationField accelerationField) {
//	accelerationField_ = accelerationField;
//}
//
//Particle* Particle::Create(const std::string& filename, Emitter emitter) {
//	Particle* model = new Particle;
//	model->Initialize(filename, emitter);
//	return model;
//}
//
//void Particle::LoadTexture(const std::string& filename) {
//	modelData = TextureManager::LoadObjFile("resources/plane.obj");
//	textureManager_ = TextureManager::GetInstance();
//	textureManager_->Initialize();
//	texture_ = textureManager_->Load(filename);
//}
//
//void Particle::CreateInstanceSRV() {
//
//	instancing_ = textureManager_->ParticleLoad(instancingResouce_.Get(), instanceCount);
//
//	//descriptorSizeSRV = Engine::GetDevice()->GetDescriptorHandleIncrementSize(
//	//    D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
//
//	//D3D12_SHADER_RESOURCE_VIEW_DESC instancingSrvDesc{};
//	//instancingSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
//	//instancingSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
//	//instancingSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
//	//instancingSrvDesc.Buffer.FirstElement = 0;
//	//instancingSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
//	//instancingSrvDesc.Buffer.NumElements = instanceCount;
//	//instancingSrvDesc.Buffer.StructureByteStride = sizeof(ParticleForGPU);
//	//instancingSrvHandelCPU = Engine::GetCPUDescriptorHandle(Engine::GetSRV().Get(), descriptorSizeSRV, 2);
//	//instancingSrvHandelGPU = Engine::GetGPUDescriptorHandle(Engine::GetSRV().Get(), descriptorSizeSRV, 2);
//	//Engine::GetDevice()->CreateShaderResourceView(instancingResouce_.Get(), &instancingSrvDesc, instancingSrvHandelCPU);
//
//}
//
//Particle_ Particle::MakeNewParticle(std::mt19937& randomEngine, const TransForm transform) {
//	std::uniform_real_distribution<float> distribution(-1.0, 1.0);
//	std::uniform_real_distribution<float> distTime(1.0, 3.0);
//	Particle_ particle;
//	particle.transform.scale = transform.scale;
//	particle.transform.rotate = transform.rotate;
//	particle.transform.translate = transform.translate + distribution(randomEngine);
//	particle.velocity = {
//	    distribution(randomEngine), distribution(randomEngine), distribution(randomEngine)};
//	if (isColor) {
//		particle.color = color_;
//	} else {
//		particle.color = {
//		    distribution(randomEngine), distribution(randomEngine), distribution(randomEngine),
//		    1.0f};
//	}
//	particle.lifeTime = distTime(randomEngine);
//	particle.currentTime = 0;
//	return particle;
//}
//
//std::list<Particle_> Particle::Emission(const Emitter& emitter, std::mt19937& randomEngine) {
//	std::list<Particle_> particles;
//	for (uint32_t count = 0; count < emitter.count; ++count) {
//		particles.push_back(MakeNewParticle(randomEngine, emitter.transform));
//	}
//	return particles;
//}