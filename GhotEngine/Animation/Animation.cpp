#include "Animation.h"


Matio::Matio()
{
}

Matio::~Matio()
{
}


void Matio::Initialize(ModelData modeldata, Animation animation){
    CreateResource(modelData);
}

Animation Matio::LoadAnimationFile(const std::string& directoryPath, const std::string& filename)
{
    Animation animation;
    Assimp::Importer importer;
    std::string filePath = directoryPath + "/" + filename;
    const aiScene* scene = importer.ReadFile(filePath.c_str(), 0);
    assert(scene->mNumAnimations != 0);//アニメーションがない
    aiAnimation* animationAssimp = scene->mAnimations[0];//最初のアニメーションだけ採用。もちろん複数対応することに越したとこはない
    animation.duration = float(animationAssimp->mDuration / animationAssimp->mTicksPerSecond);//時間の単位を秒に変換


    //assimpでは個々のNodeのAnimationをchannlと呼んでいるのでchannelを回してNodeAnimationの情報を取ってくる
    for (uint32_t channelIndex = 0; channelIndex < animationAssimp->mNumChannels; ++channelIndex) {
        aiNodeAnim* nodeAnimationAssimp = animationAssimp->mChannels[channelIndex];
        NodeAnimation& nodeAnimation = animation.nodeAnimations[nodeAnimationAssimp->mNodeName.C_Str()];
        for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumPositionKeys; ++keyIndex) {
            aiVectorKey& keyAssimp = nodeAnimationAssimp->mPositionKeys[keyIndex];
            KeyframeVector3 keyframe;
            keyframe.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond);//ここも秒に変換
            keyframe.value = { -keyAssimp.mValue.x,keyAssimp.mValue.y,keyAssimp.mValue.z };//右手→左手
            nodeAnimation.translate.push_back(keyframe);
        }
        for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumRotationKeys; ++keyIndex) {
            aiQuatKey& keyAssimp = nodeAnimationAssimp->mRotationKeys[keyIndex];
            KeyframeQuaternion keyframe;
            keyframe.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond);//ここも秒に変換
            keyframe.value = { keyAssimp.mValue.x,-keyAssimp.mValue.y,-keyAssimp.mValue.z,keyAssimp.mValue.w };//右手→左手
            nodeAnimation.rotate.push_back(keyframe);
        }
        for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumScalingKeys; ++keyIndex) {
            aiVectorKey& keyAssimp = nodeAnimationAssimp->mScalingKeys[keyIndex];
            KeyframeVector3 keyframe;
            keyframe.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond);//ここも秒に変換
            keyframe.value = { -keyAssimp.mValue.x,keyAssimp.mValue.y,keyAssimp.mValue.z };//右手→左手
            nodeAnimation.scale.push_back(keyframe);
        }
    }

    //解析完了
    return animation;
}

Vector3 Matio::CalculateValue(const std::vector<KeyframeVector3>& keyframes, float time){
    assert(!keyframes.empty());//キーがないものは返す値が分らないのでエラー
    if (keyframes.size() == 1 || time <= keyframes[0].time) {//キーが1つか、時刻のがキーフレーム前なら最初の値とする
        return keyframes[0].value;
    }

    for (size_t index = 0; index < keyframes.size() - 1; ++index) {
        size_t nextIndex = index + 1;
        //indexとnextIndexの2つのkeyframeを取得して範囲内に時刻があるのかを判定
        if (keyframes[index].time <= time && time <= keyframes[nextIndex].time) {
            //範囲内を補間する
            float t = (time - keyframes[index].time) / (keyframes[nextIndex].time - keyframes[index].time);
            return Lerp(keyframes[index].value, keyframes[nextIndex].value, t);
        }
    }

    //ここまでできた場合は一番後の時刻よりも後ろなので最後の値をかえすことになる
    return (*keyframes.rbegin()).value;
}
//くおたにおんも作る

Quaternion Matio::CalculateValue(const std::vector<KeyframeQuaternion>& keyframes, float time) {
    assert(!keyframes.empty());//キーがないものは返す値が分らないのでエラー
    if (keyframes.size() == 1 || time <= keyframes[0].time) {//キーが1つか、時刻のがキーフレーム前なら最初の値とする
        return keyframes[0].value;
    }

    for (size_t index = 0; index < keyframes.size() - 1; ++index) {
        size_t nextIndex = index + 1;
        //indexとnextIndexの2つのkeyframeを取得して範囲内に時刻があるのかを判定
        if (keyframes[index].time <= time && time <= keyframes[nextIndex].time) {
            //範囲内を補間する
            float t = (time - keyframes[index].time) / (keyframes[nextIndex].time - keyframes[index].time);
            return Lerp(keyframes[index].value, keyframes[nextIndex].value, t);
        }
    }

    //ここまでできた場合は一番後の時刻よりも後ろなので最後の値をかえすことになる
    return (*keyframes.rbegin()).value;

}

void Matio::CreateResource(ModelData modelData){
    // Instancing用のTransformationMatrixResourceを作る
    resource_.instancingResource = CreateResource::CreateBufferResource(sizeof(ParticleForGPU) );
    // 書き込むためのアドレスを取得
    resource_.instancingResource->Map(0, nullptr, reinterpret_cast<void**>(&transformData_));
    // 単位行列を書き込む
        transformData_->WVP = MakeIdentityMatrix();
        transformData_->World = MakeIdentityMatrix();
        //transformData_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

    // VertexResource
    resource_.vertexResource = CreateResource::CreateBufferResource(sizeof(VertexData) * modelData.vertices.size());
    // VertexBufferView
    AnimationVertexBufferView_.BufferLocation = resource_.vertexResource->GetGPUVirtualAddress();
    // 使用するリソースのサイズは頂点サイズ
    AnimationVertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * modelData.vertices.size());
    // 1頂点あたりのサイズ
    AnimationVertexBufferView_.StrideInBytes = sizeof(VertexData);

    // 頂点リソースにデータを書き込む
    VertexData* vertexData = nullptr;
    // 書き込むためのアドレスを取得
    resource_.vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
    std::memcpy(vertexData, modelData.vertices.data(), sizeof(VertexData) * modelData.vertices.size()); // 頂点データをリソースにコピー
    resource_.materialResource = CreateResource::CreateBufferResource(sizeof(Material));
    // データを書き込む
    Material* materialData = nullptr;
    // アドレスを取得
    resource_.materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
    materialData->color = { 1.0f,1.0f,1.0f,1.0f };

    resource_.wvpResource = CreateResource::CreateBufferResource(sizeof(TransformationMatrix));

}

void Matio::Playback(WorldTransform& worldTransform, CameraRole& camera) {
    animationTime += 1.0f / 60.0f;//時間を進める。1/60で計測した時間を使って可変フレーム対応する法が望ましい
    animationTime = std::fmod(animationTime, animation.duration);//最後まで行ったら最初からリピート再生。リピート再生しなくてもよい
    NodeAnimation& rootNodeAnimation = animation.nodeAnimations[modelData.rootNode.name];//rootNodeのAnimationを取得
    Vector3 translate = CalculateValue(rootNodeAnimation.translate, animationTime);//指定時間の値の取得
    Quaternion rotate = CalculateValue(rootNodeAnimation.rotate, animationTime);
    Vector3 scale = CalculateValue(rootNodeAnimation.scale, animationTime);

    Matrix4x4 localMatrix = MakeAffineMatrix(scale, rotate, translate);
    Matrix4x4 worldViewProjectionMatrix = Multiply(localMatrix, Multiply(worldTransform.worldMatrix, Multiply(camera.matView, camera.matProjection)));

    transformData_->WVP = worldViewProjectionMatrix;
    transformData_->World = Multiply(localMatrix, worldTransform.worldMatrix);


    TransformationMatrix* wvp = {};
    worldViewProjectionMatrix = Multiply(localMatrix, Multiply(worldTransform.worldMatrix, Multiply(camera.matView, camera.matProjection)));
    resource_.wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvp));
    wvp->WVP = worldViewProjectionMatrix;
    wvp->World = Multiply(localMatrix, worldTransform.worldMatrix);


    Property property = GraphicsPipeline::GetInstance()->GetPSO().Object3D;

    // Rootsignatureを設定。PSOに設定してるけど別途設定が必要
    DirectXCommon::GetCommandList()->SetGraphicsRootSignature(property.rootSignature_.Get());
    DirectXCommon::GetCommandList()->SetPipelineState(property.graphicsPipelineState_.Get()); // PSOを設定

    // 形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
    DirectXCommon::GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    // マテリアルCBufferの場所を設定
    DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(0, resource_.materialResource->GetGPUVirtualAddress());

    // wvp用のCBufferの場所を設定
//    worldTransform.TransferMatrix(resource_.wvpResource, camera);
    DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(1, resource_.wvpResource->GetGPUVirtualAddress());
    DirectXCommon::GetCommandList()->SetGraphicsRootDescriptorTable(2, SrvManager::GetInstance()->GetGPUHandle(texHandle_));

    DirectXCommon::GetCommandList()->IASetVertexBuffers(0, 1, &AnimationVertexBufferView_); // VBVを設定

    // 描画。(DrawCall/ドローコール)。
    DirectXCommon::GetCommandList()->DrawInstanced(UINT(modelData.vertices.size()), 1, 0, 0);
}


