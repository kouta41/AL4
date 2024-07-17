#include "Skybox.h"

void Skybox::Initialize(){
	resource_.vertexResource = CreateResource::CreateBufferResource(sizeof(VertexData) * 24);

	VBV = CreateResource::CreateVertexBufferView(resource_.vertexResource, sizeof(VertexData) * 24, 24);

	// 頂点リソースにデータを書き込む
	VertexData* vertexData = nullptr;
	// 書き込むためのアドレスを取得
	resource_.vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));


	resource_.indexResource = CreateResource::CreateBufferResource(sizeof(uint32_t) * 36);
	IBV = CreateResource::CreateIndexBufferView(resource_.indexResource, sizeof(uint32_t) * 36);

	// インデックスリソースにデータを書き込む
	uint32_t* indexData = nullptr;
	resource_.indexResource->Map(0, nullptr, reinterpret_cast<void**>(&indexData));


#pragma region 頂点データ
	//右面。描画インデックスは[0,1,2][2,1,3]で内側を向く
	vertexData[0].position = { 1.0f,1.0f,1.0f,1.0f };//{↑、↑、↑}
	vertexData[1].position = { 1.0f,1.0f,-1.0f,1.0f };
	vertexData[2].position = { 1.0f,-1.0f,1.0f,1.0f };
	vertexData[3].position = { 1.0f,-1.0f,-1.0f,1.0f };
	//左面。描画インデックスは[4,5,6][6,5,7]で内側を向く
	vertexData[4].position = { -1.0f,1.0f,-1.0f,1.0f };
	vertexData[5].position = { -1.0f,1.0f,1.0f,1.0f };
	vertexData[6].position = { -1.0f,-1.0f,-1.0f,1.0f };
	vertexData[7].position = { -1.0f,-1.0f,1.0f,1.0f };
	//前面。描画インデックスは[8,9,10][10,9,11]で内側を向く
	vertexData[8].position = { -1.0f,1.0f,1.0f,1.0f };
	vertexData[9].position = { 1.0f,1.0f,1.0f,1.0f };
	vertexData[10].position = { -1.0f,-1.0f,1.0f,1.0f };
	vertexData[11].position = { 1.0f,-1.0f,1.0f,1.0f };

	//後面。描画インデックスは[12,13,14][14,13,15]で内側を向く
	vertexData[12].position = { 1.0f,1.0f,-1.0f,1.0f }; 
	vertexData[13].position = { -1.0f,1.0f,-1.0f,1.0f };
	vertexData[14].position = { 1.0f,-1.0f,-1.0f,1.0f };
	vertexData[15].position = { -1.0f,-1.0f,-1.0f,1.0f };

	//上面。描画インデックスは[16,17,18][18,17,19]で内側を向く
	vertexData[16].position = { -1.0f,1.0f,-1.0f,1.0f };
	vertexData[17].position = { 1.0f,1.0f,-1.0f,1.0f };
	vertexData[18].position = { -1.0f,1.0f,1.0f,1.0f };
	vertexData[19].position = { 1.0f,1.0f,1.0f,1.0f };
	////下面。描画インデックスは[20,21,22][22,21,23]で内側を向く
	vertexData[20].position = { 1.0f,-1.0f,-1.0f,1.0f };
	vertexData[21].position = { -1.0f,-1.0f,-1.0f,1.0f };
	vertexData[22].position = { 1.0f,-1.0f,1.0f,1.0f };
	vertexData[23].position = { -1.0f,-1.0f,1.0f,1.0f };

	// インデックスデータ
	// 右側面
	indexData[0] = 0;    indexData[1] = 1;   indexData[2] = 2;
	indexData[3] = 2;	 indexData[4] = 1;	 indexData[5] = 3;
	// 左側面
	indexData[6] = 4;	 indexData[7] = 5;	 indexData[8] = 6;
	indexData[9] = 6;	 indexData[10] = 5;	 indexData[11] = 7;
	// 前面
	indexData[12] = 8;	 indexData[13] = 9;	 indexData[14] = 10;
	indexData[15] = 10;	 indexData[16] = 9;	 indexData[17] = 11;
	// 背面
	indexData[18] = 12;	 indexData[19] = 13;  indexData[20] = 14;
	indexData[21] = 14;	 indexData[22] = 13;  indexData[23] = 15;
	// 上面
	indexData[24] = 16;	 indexData[25] = 17; indexData[26] = 18;
	indexData[27] = 18;	 indexData[28] = 17; indexData[29] = 19;
	// 底面
	indexData[30] = 20;	 indexData[31] = 21; indexData[32] = 22;
	indexData[33] = 22;	 indexData[34] = 21; indexData[35] = 23;


#pragma endregion

	resource_.materialResource = CreateResource::CreateBufferResource(sizeof(Material));
	// データを書き込む
	Material* materialData = nullptr;
	// アドレスを取得
	resource_.materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	materialData->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	materialData->enableLighting = true;

	resource_.wvpResource = CreateResource::CreateBufferResource(sizeof(TransformationMatrix));

	// 平行光源用のリソース
	resource_.directionalLightResource = CreateResource::CreateBufferResource(sizeof(DirectionalLight));
	// データを書き込む
	DirectionalLight* directionalLightData = nullptr;
	// 書き込むためのアドレスを取得
	resource_.directionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData));
	directionalLightData->color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData->direction = Normalize({ 0.0f, -1.0f, 0.0f });
	directionalLightData->intensity = 1.0f;

}

void Skybox::Draw(WorldTransform worldTransform, CameraRole cameraRole, uint32_t texHandle){

	worldTransform.TransferMatrix();

	Property property_ = GraphicsPipeline::GetInstance()->GetPSO().Skybox;


	// Rootsignatureを設定。PSOに設定してるけど別途設定が必要
	DirectXCommon::GetCommandList()->SetGraphicsRootSignature(property_.rootSignature_.Get());
	DirectXCommon::GetCommandList()->SetPipelineState(property_.graphicsPipelineState_.Get()); // PSOを設定

	// 形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	DirectXCommon::GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// マテリアルCBufferの場所を設定
	DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(0, resource_.materialResource->GetGPUVirtualAddress());
	DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(1, worldTransform.constBuff->GetGPUVirtualAddress());
	DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(2, cameraRole.constBuff_->GetGPUVirtualAddress());
	// wvp用のCBufferの場所を設定
	//worldTransform.TransferMatrix();
	DirectXCommon::GetCommandList()->SetGraphicsRootDescriptorTable(3, SrvManager::GetInstance()->GetDescriptorHeapForGPU(texHandle));
	DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(4, resource_.directionalLightResource->GetGPUVirtualAddress());

	DirectXCommon::GetCommandList()->IASetVertexBuffers(0, 1, &VBV); // VBVを設定
	DirectXCommon::GetCommandList()->IASetIndexBuffer(&IBV); // IBVの設定
	// 描画。(DrawCall/ドローコール)。
	DirectXCommon::GetCommandList()->DrawIndexedInstanced(36, 1, 0, 0,0);
}
