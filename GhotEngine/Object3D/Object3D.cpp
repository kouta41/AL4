#include "Object3D.h"

void Object3DPlacer::Initialize()
{
	resource_.materialResource = CreateResource::CreateBufferResource(sizeof(Material));
	resource_.materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->color = { 1.0f,1.0f,1.0f,1.0f };
	materialData_->enableLighting = true;
	materialData_->shininess = 1.0f;
	materialData_->environmentCoefficient = 1.0f;

	// 平行光源用のリソース
	resource_.directionalLightResource = CreateResource::CreateBufferResource(sizeof(DirectionalLight));
	// 書き込むためのアドレスを取得
	resource_.directionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
	directionalLightData_->color = { color_ };
	directionalLightData_->direction = Normalize({ 0.0f, -1.0f, 0.0f });
	directionalLightData_->intensity = 1.0f;

	resource_.wvpResource = CreateResource::CreateBufferResource(sizeof(TransformationMatrix));


	//カメラリソース
	resource_.cameraResource = CreateResource::CreateBufferResource(sizeof(CameraForGPU));
	resource_.cameraResource->Map(0, nullptr, reinterpret_cast<void**>(&cameraData_));
	cameraData_->worldPosition = { 0,0,0 };

}

void Object3DPlacer::Draw(WorldTransform worldTransform, CameraRole camera)
{

	
	property_ = GraphicsPipeline::GetInstance()->GetPSO().Object3D;

	// Rootsignatureを設定。PSOに設定してるけど別途設定が必要
	DirectXCommon::GetCommandList()->SetGraphicsRootSignature(property_.rootSignature_.Get());
	DirectXCommon::GetCommandList()->SetPipelineState(property_.graphicsPipelineState_.Get()); // PSOを設定

	// 形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	DirectXCommon::GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// マテリアルCBufferの場所を設定
	DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(0, resource_.materialResource->GetGPUVirtualAddress());
	DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(1, worldTransform.wvpResource->GetGPUVirtualAddress());
	DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(2, camera.constBuff_->GetGPUVirtualAddress());
	// wvp用のCBufferの場所を設定
	//worldTransform.TransferMatrix();
	DirectXCommon::GetCommandList()->SetGraphicsRootDescriptorTable(3, SrvManager::GetInstance()->GetDescriptorHeapForGPU(texHandle_));
	DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(4, resource_.directionalLightResource->GetGPUVirtualAddress());
	DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(5, resource_.cameraResource->GetGPUVirtualAddress());

	ImGui::Begin("player_model");
	if (ImGui::TreeNode("worldTransform")) {
		ImGui::DragFloat3("translate", &worldTransform.translate.x, 0.1f, 100, 100);
		ImGui::DragFloat3("rotate", &worldTransform.rotate.x, 0.01f, -6.28f, 6.28f);
		ImGui::DragFloat3("scale", &worldTransform.scale.x, 0.01f, 0, 10);
		ImGui::TreePop();
	}
	ImGui::End();


	if (model_) {
		model_->Draw();
	}
}