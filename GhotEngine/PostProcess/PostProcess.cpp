#include "PostProcess.h"

PostProcess::PostProcess()
{
}
void PostProcess::Init()
{



}

void PostProcess::Update()
{
}

void PostProcess::Draw() {
	property_ = GraphicsPipeline::GetInstance()->GetPSO().CopyImage;

	//DirectXCommon* sDirectXCommon = DirectXCommon::GetInstance();
	// Rootsignatureを設定。PSOに設定してるけど別途設定が必要
	DirectXCommon::GetCommandList()->SetGraphicsRootSignature(property_.rootSignature_.Get());
	DirectXCommon::GetCommandList()->SetPipelineState(property_.graphicsPipelineState_.Get()); // PSOを設定

	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	//DirectXCommon::GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// マテリアルCBufferの場所を設定
	DirectXCommon::GetCommandList()->SetGraphicsRootDescriptorTable(0, SrvManager::GetInstance()->GetDescriptorHeapForGPU(SrvManager::GetInstance()->GetIndex()));
	// 描画（DrawCall/ドローコール）
	DirectXCommon::GetCommandList()->DrawInstanced(3, 1, 0, 0);
}
