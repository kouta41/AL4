#include "Object3D.h"

void Object3DPlacer::Initialize()
{
	resource_.materialResource = CreateResource::CreateBufferResource(sizeof(Material));
	resource_.materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->color = { 1.0f,1.0f,1.0f,1.0f };
	materialData_->enableLighting = false;
	materialData_->shininess = 70.0f;

	// 平行光源用のリソース
	resource_.directionalLightResource = CreateResource::CreateBufferResource(sizeof(DirectionalLight));
	// 書き込むためのアドレスを取得
	resource_.directionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
	directionalLightData_->color = { color_ };
	directionalLightData_->direction = Normalize({ 0.0f, -1.0f, 0.0f });
	directionalLightData_->intensity = 1.0f;

	resource_.wvpResource = CreateResource::CreateBufferResource(sizeof(TransformationMatrix));
}

void Object3DPlacer::Draw(WorldTransform worldTransform, CameraRole camera)
{

	/*if (lighting_ == nullptr) {
		property_ = GraphicsPipeline::GetInstance()->GetPSO().Object3D;
	}
	else {
		if (lighting_->GetLightType() == None) {
		}
		else if (lighting_->GetLightType() == Point) {
			property_ = GraphicsPipeline::GetInstance()->GetPSO().PointLight;
		}
		else if (lighting_->GetLightType() == Spot) {
			property_ = GraphicsPipeline::GetInstance()->GetPSO().SpotLight;
		}
	}*/
	property_ = GraphicsPipeline::GetInstance()->GetPSO().Object3D;

	// Rootsignatureを設定。PSOに設定してるけど別途設定が必要
	DirectXCommon::GetCommandList()->SetGraphicsRootSignature(property_.rootSignature_.Get());
	DirectXCommon::GetCommandList()->SetPipelineState(property_.graphicsPipelineState_.Get()); // PSOを設定

	// 形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	DirectXCommon::GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// マテリアルCBufferの場所を設定
	DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(0, resource_.materialResource->GetGPUVirtualAddress());

	// wvp用のCBufferの場所を設定
	worldTransform.TransferMatrix(resource_.wvpResource, camera);
	DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(1, resource_.wvpResource->GetGPUVirtualAddress());
	DirectXCommon::GetCommandList()->SetGraphicsRootDescriptorTable(2, SrvManager::GetInstance()->GetGPUHandle(texHandle_));
	// 平行光源
	//DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(3, resource_.directionalLightResource->GetGPUVirtualAddress());

	/*if (lighting_) {
		lighting_->CreateCommand();
	}*/

	if (model_) {
		model_->Draw();
	}
}