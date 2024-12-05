/// <summary>
/// スプライト
/// </summary>
#include "Sprite.h"
#include "Vector4.h"
#include <Object3D.h>

CameraRole Sprite::camera_;

Sprite::~Sprite()
{

}

void Sprite::StaticInitialize()
{
	camera_.Initialize();
}

void Sprite::StaticUpdate()
{
	camera_.UpdateMatrix();
}

/// <summary>
/// 初期化
/// </summary>
void Sprite::Initialize(uint32_t texHandle) {

	worldTransform_.Initialize();

	texHandle_ = texHandle;


	// 平行光源用のリソース
	resource_.directionalLightResource = CreateResource::CreateBufferResource(sizeof(DirectionalLight));
	// 書き込むためのアドレスを取得
	resource_.directionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
	directionalLightData_->color = { color_ };
	directionalLightData_->direction = Normalize({ 0.0f, -1.0f, 0.0f });
	directionalLightData_->intensity = 1.0f;


	// indexResourceSprite
	resource_.indexResource = CreateResource::CreateBufferResource(sizeof(uint32_t) * 6);
	// バッファービューを作成
	IBV_ = CreateResource::CreateIndexBufferView(resource_.indexResource, sizeof(uint32_t) * 6);
	// インデックスリソースにデータを書き込む
	uint32_t* indexDataSprite = nullptr;
	resource_.indexResource->Map(0, nullptr, reinterpret_cast<void**>(&indexDataSprite));
	indexDataSprite[0] = 0;
	indexDataSprite[1] = 1;
	indexDataSprite[2] = 2;
	indexDataSprite[3] = 1;
	indexDataSprite[4] = 3;
	indexDataSprite[5] = 2;


	resource_.materialResource = CreateResource::CreateBufferResource(sizeof(Vector4));
	// データを書き込む
	// アドレスを取得
	resource_.materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	// 赤
	*materialData_ = Vector4(0.0f, 0.0f, 0.0f, 0.0f);

	resource_.wvpResource = CreateResource::CreateBufferResource(sizeof(TransformationMatrix));



	AdjustTextureSize(texHandle_);

	textureSize_ = size_;

}

/// <summary>
/// 頂点データ作成
/// </summary>
void Sprite::CreateVertex()
{

	resource_.vertexResource = CreateResource::CreateBufferResource(sizeof(VertexData) * 4);

	VBV_ = CreateResource::CreateVertexBufferView(resource_.vertexResource, sizeof(VertexData) * 4, 4);

	resource_.wvpResource = CreateResource::CreateBufferResource(sizeof(TransformationMatrix));

	// 頂点データの設定
	VertexData* vertexDataSprite = nullptr;
	resource_.vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexDataSprite));

	float left = (0.0f - anchorPoint_.x) * size_.x;
	float right = (1.0f - anchorPoint_.x) * size_.x;
	float top = (0.0f - anchorPoint_.y) * size_.y;
	float bottom = (1.0f - anchorPoint_.y) * size_.y;

	float tex_left = textureLeftTop_.x / size_.x;
	float tex_right = (textureLeftTop_.x + textureSize_.x) / size_.x;
	float tex_top = textureLeftTop_.y / size_.y;
	float tex_bottom = (textureLeftTop_.y + textureSize_.y) / size_.y;


	// 1枚目の三角形
	//vertexDataSprite[1].position = { left, bottom,0.0f, 1.0f }; // 左下
	//vertexDataSprite[1].texcoord = { tex_left, tex_bottom };

	//vertexDataSprite[0].position = { left, top, 0.0f, 1.0f }; // 左上
	//vertexDataSprite[0].texcoord = { tex_left, tex_top };

	//vertexDataSprite[3].position = { right, bottom, 0.0f,1.0f }; // 右下
	//vertexDataSprite[3].texcoord = { tex_right, tex_bottom };

	//// 2枚目の三角形
	//vertexDataSprite[2].position = { right, top, 0.0f, 1.0f }; // 右上
	//vertexDataSprite[2].texcoord = { tex_right, tex_top };

	vertexDataSprite[0].position = { left, bottom,0.0f, 1.0f }; // 左下
	vertexDataSprite[0].texcoord = { tex_left, tex_bottom };

	vertexDataSprite[1].position = { left, top, 0.0f, 1.0f }; // 左上
	vertexDataSprite[1].texcoord = { tex_left, tex_top };

	vertexDataSprite[2].position = { right, bottom, 0.0f,1.0f }; // 右下
	vertexDataSprite[2].texcoord = { tex_right, tex_bottom };

	// 2枚目の三角形
	vertexDataSprite[3].position = { right, top, 0.0f, 1.0f }; // 右上
	vertexDataSprite[3].texcoord = { tex_right, tex_top };
}


/// <summary>
/// スプライト生成
/// </summary>
/// <param name="position"></param>
/// <returns></returns>
Sprite* Sprite::Create(uint32_t texHandle, Vector2 position, Vector4 color)
{
	
	Sprite* sprite = new Sprite;
	sprite->Initialize(texHandle);
	sprite->SetPosition(position);
	sprite->SetColor(color);
	
	return sprite;
}

void Sprite::AdjustTextureSize(uint32_t texHandle)
{
	const DirectX::TexMetadata& metadata = TextureManager::GetInstance()->GetMetaData(texHandle);

	size_.x = static_cast<float>(metadata.width);
	size_.y = static_cast<float>(metadata.height);

}

/// <summary>
/// 描画
/// </summary>
/// <param name="v"></param>
/// <param name="t"></param>
void Sprite::Draw()
{

	CreateVertex();
	//worldTransform_.translate.x = GetPosition().x;
	//worldTransform_.translate.y = GetPosition().y;
	worldTransform_.UpdateMatrix();

	Property property = GraphicsPipeline::GetInstance()->GetPSO().Sprite2D;

	// Rootsignatureを設定。PSOに設定してるけど別途設定が必要
	DirectXCommon::GetCommandList()->SetGraphicsRootSignature(property.rootSignature_.Get());
	DirectXCommon::GetCommandList()->SetPipelineState(property.graphicsPipelineState_.Get()); // PSOを設定
	DirectXCommon::GetCommandList()->IASetVertexBuffers(0, 1, &VBV_); // VBVを設定	
	DirectXCommon::GetCommandList()->IASetIndexBuffer(&IBV_); // IBVの設定
	// 形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	DirectXCommon::GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// マテリアルCBufferの場所を設定
	DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(0, resource_.materialResource->GetGPUVirtualAddress());
	// wvp用のCBufferの場所を設定
	DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(1, worldTransform_.constBuff->GetGPUVirtualAddress());
	//DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(1, resource_.wvpResource->GetGPUVirtualAddress());
	DirectXCommon::GetCommandList()->SetGraphicsRootDescriptorTable(2, SrvManager::GetInstance()->GetDescriptorHeapForGPU(texHandle_));
	DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(3, resource_.directionalLightResource->GetGPUVirtualAddress());
	DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(4, camera_.sConstBuff->GetGPUVirtualAddress());


	// 描画。(DrawCall/ドローコール)。3頂点で1つのインスタンス。インスタンスについては今後
	DirectXCommon::GetCommandList()->DrawIndexedInstanced(6, 1, 0, 0, 0);

#ifdef RELEASE

	ImGui::Begin("worldTransform");
	if (ImGui::TreeNode("worldTransform")) {
		ImGui::DragFloat3("translate", &worldTransform_.translate.x, 0.1f, 100, 100);
		ImGui::DragFloat3("rotate", &worldTransform_.rotate.x, 0.01f, 100, 100);
		ImGui::DragFloat3("scale", &worldTransform_.scale.x, 0.01f, 100, 100);


		ImGui::TreePop();
	}
	ImGui::End();
#endif // RELEASE


}
