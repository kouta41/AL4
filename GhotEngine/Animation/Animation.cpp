#include "Animation.h"


Matio::Matio()
{
}

Matio::~Matio()
{
}


void Matio::Initialize(ModelData modeldata, Animation animation){

    animation_ = animation;
    modelData_ = modeldata;
  

    skeleton_ = CreateSkeleton(modelData_.rootNode); // skeleton

    skinCluster_ = CreateSkinCluster(modelData_, skeleton_);//skinCluster

    CreateResource();
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
        ///
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
            keyframe.value = { keyAssimp.mValue.x,keyAssimp.mValue.y,keyAssimp.mValue.z };//右手→左手
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
            return SLerp(keyframes[index].value, keyframes[nextIndex].value, t);
        }
    }

    //ここまでできた場合は一番後の時刻よりも後ろなので最後の値をかえすことになる
    return (*keyframes.rbegin()).value;

}

Skeleton Matio::CreateSkeleton(const Node& rootNode){
    Skeleton skeleton;
    skeleton.root = CreateJoint(rootNode, {}, skeleton.joints);

    //名前とindexのマッピングを行いアクセスしやすくする
    for (const Joint& joint : skeleton.joints) {
        skeleton.jointMap.emplace(joint.name, joint.index);
    }
    return skeleton;
}

int32_t Matio::CreateJoint(const Node& node, const std::optional<int32_t>& parent, std::vector<Joint>& joints){
    Joint joint;
    joint.name = node.name;
    joint.localMatrix = node.localMatrix;
    joint.skeletonSpaceMatrix = MakeIdentityMatrix();
    joint.transform = node.transform;
    joint.index = int32_t(joints.size());//現在登録されている数をIndexに
    joint.parent = parent;
    joints.push_back(joint);//Skeletonのjoint列に追加
    for (const Node& child : node.children) {
        //子Jointwoを作成し、そのIndexを登録
        int32_t childIndex = CreateJoint(child, joint.index, joints);
        joints[joint.index].children.push_back(childIndex);
    }
    //自身のIndexを返す
    return joint.index;
}

SkinCluster Matio::CreateSkinCluster(ModelData& modelData, const Skeleton& skeleton){
    SkinCluster skinCluster;
    //palette用のResourceを確保
    skinCluster.paletteResource = CreateResource::CreateBufferResource(sizeof(WeelForGPU) * skeleton.joints.size());
    WeelForGPU* mappedPalette = nullptr;
    skinCluster.paletteResource->Map(0, nullptr, reinterpret_cast<void**>(&mappedPalette));
    skinCluster.mappedPalette = { mappedPalette,skeleton.joints.size() };
    srvIndex_ = SrvManager::GetInstance()->GetIndex();
    SrvManager::GetInstance()->ShiftIndex(); // srvのindexの位置を空いてる次にずらす
    skinCluster.paletteSrvHandle.first = SrvManager::GetInstance()->GetDescriptorHeapForCPU(srvIndex_);
    skinCluster.paletteSrvHandle.second = SrvManager::GetInstance()->GetDescriptorHeapForGPU(srvIndex_);

    //palette用のsrvを作成。StructuredBufferでアクセスしやすいようにする
    D3D12_SHADER_RESOURCE_VIEW_DESC paletteSrvDesc{};
    paletteSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
    paletteSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    paletteSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
    paletteSrvDesc.Buffer.FirstElement = 0;
    paletteSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
    paletteSrvDesc.Buffer.NumElements = UINT(skeleton.joints.size());
    paletteSrvDesc.Buffer.StructureByteStride = sizeof(WeelForGPU);
    DirectXCommon::GetInstance()->GetDevice()->CreateShaderResourceView(skinCluster.paletteResource.Get(), &paletteSrvDesc, skinCluster.paletteSrvHandle.first);

    //influence用のResourceを確保。頂点ごとにinfluence情報を追加できるようにする
    skinCluster.influenceResource = CreateResource::CreateBufferResource(sizeof(VertexInfluence) * modelData.vertices.size());
    VertexInfluence* mappedInfluence = nullptr;
    skinCluster.influenceResource->Map(0, nullptr, reinterpret_cast<void**>(&mappedInfluence));
    std::memset(mappedInfluence, 0, sizeof(VertexInfluence) * modelData.vertices.size());//0埋め。weightを0にしておく
    skinCluster.mappedInfluence = { mappedInfluence,modelData.vertices.size() };

    //Influence用のVBVを作成
    skinCluster.influenceBufferView.BufferLocation = skinCluster.influenceResource->GetGPUVirtualAddress();
    skinCluster.influenceBufferView.SizeInBytes = UINT(sizeof(VertexInfluence) * modelData.vertices.size());
    skinCluster.influenceBufferView.StrideInBytes = sizeof(VertexInfluence);

    //InveresBindPoseMatrixを格納する場所を作成して、単位行列で埋める
    skinCluster.inverseBindPoseMatrices.resize(skeleton.joints.size());
    std::generate(skinCluster.inverseBindPoseMatrices.begin(), skinCluster.inverseBindPoseMatrices.end(), MakeIdentityMatrix);

    //ModelDataを解析してInfluenceを埋める
    for (const auto& jointWeight : modelData.skinClusterData) {//modelのskinClusterの情報を解析
        auto it = skeleton.jointMap.find(jointWeight.first);//jointWeight.firstはjoint名なので、skeletonに対象となるjointが含まれているのか判断
        if (it == skeleton.jointMap.end()) {//そんな名前のjointは存在しない。なので次に回す
            continue;
        }
        //(*it).secondにはjointのindexが入っているので、該当のindexのinverseBindPoseMatrixを代入
        skinCluster.inverseBindPoseMatrices[(*it).second] = jointWeight.second.inverseBindPoseMatrix;
        for (const auto& vertexWeight : jointWeight.second.vertexWeights) {
            auto& currentInfluence = skinCluster.mappedInfluence[vertexWeight.vertexIndex];//該当のvertexIndexのinfluence情報を参照しておく
            for (uint32_t index = 0; index < kNumMakInfluence; ++index) {//空いているところに入れる
                if (currentInfluence.weights[index] == 0.0f) {//weght==0が空いてる状態なので、その場所にweightとjointのindexを代入
                    currentInfluence.weights[index] = vertexWeight.weight;
                    currentInfluence.jointIndices[index] = (*it).second;
                    break;
                }
            }
        }
    }

    return skinCluster;
}

void Matio::CreateResource(){
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
  //  resource_.wvpResource = CreateResource::CreateBufferResource(sizeof(TransformationMatrix));


    // VertexResource
    resource_.vertexResource = CreateResource::CreateBufferResource(sizeof(VertexData) * modelData_.vertices.size());
    //VertexBufferView
    // 頂点バッファビューを作成する

    // リソースの先頭のアドレスから使う
    VertexBufferView_.BufferLocation = resource_.vertexResource->GetGPUVirtualAddress();
    // 使用するリソースのサイズは頂点サイズ
    VertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * modelData_.vertices.size());
    // 1頂点あたりのサイズ
    VertexBufferView_.StrideInBytes = sizeof(VertexData);

    // 頂点リソースにデータを書き込む
    VertexData* vertexData = nullptr;
    // 書き込むためのアドレスを取得
    resource_.vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
    std::memcpy(vertexData, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size()); // 頂点データをリソースにコピー;



    resource_.indexResource = CreateResource::CreateBufferResource(sizeof(uint32_t) * modelData_.indices.size());

    // リソースの先頭のアドレスから使う
    IndexBufferView_.BufferLocation = resource_.indexResource->GetGPUVirtualAddress();
    // 使用するリソースのサイズは頂点サイズ
    IndexBufferView_.SizeInBytes = UINT(sizeof(uint32_t) * modelData_.indices.size());
    // 1頂点あたりのサイズ
    IndexBufferView_.Format = DXGI_FORMAT_R32_UINT;

    uint32_t* index = nullptr;

    //// 書き込むためのアドレスを取得
    resource_.indexResource->Map(0, nullptr, reinterpret_cast<void**>(&index));
    std::memcpy(index, modelData_.indices.data(), sizeof(uint32_t) * modelData_.indices.size()); // 頂点データをリソースにコピー;


}

void Matio::SkeletonUpdate(Skeleton& skeleton){
    //すべてのJointを更新。親が若いので通常ループで処理を可能にしている
    for (Joint& joint : skeleton.joints) {
        joint.localMatrix = MakeAffineMatrix(joint.transform.scale, joint.transform.rotate, joint.transform.translate);
        if (joint.parent) {//親がいれば親の行列をかける
            joint.skeletonSpaceMatrix =joint.localMatrix * skeleton.joints[*joint.parent].skeletonSpaceMatrix;
        } else {//親がいなければlocalMatrixとskeletonSpeaceMatrixは一致する
            joint.skeletonSpaceMatrix = joint.localMatrix;
        }
    }
}

void Matio::SkinClusterUpdate(SkinCluster& skinCluster, const Skeleton& skeleton){
    for (size_t jointIndex = 0; jointIndex < skeleton.joints.size(); ++jointIndex) {
        assert(jointIndex < skinCluster.inverseBindPoseMatrices.size());
        skinCluster.mappedPalette[jointIndex].skeletonSpaceMatrix =
            skinCluster.inverseBindPoseMatrices[jointIndex] * skeleton.joints[jointIndex].skeletonSpaceMatrix;
        skinCluster.mappedPalette[jointIndex].skeletonSpaceInverseTransposeMatrix =
            Transpose(Inverse(skinCluster.mappedPalette[jointIndex].skeletonSpaceMatrix));
    }
}

void Matio::ApplyAnimation(Skeleton& skeleton, const Animation& animation,float animationTime){
    for (Joint& joint : skeleton.joints) {
        //対象のJointのAnimationがあれば、値の適用を行う。下記のif文はC++17から可能になった初期化文付きのif文
        if (auto it = animation.nodeAnimations.find(joint.name); it != animation.nodeAnimations.end()) {
            const NodeAnimation& rootNodeAnimation = (*it).second;
            joint.transform.translate = CalculateValue(rootNodeAnimation.translate, animationTime);
            joint.transform.rotate = CalculateValue(rootNodeAnimation.rotate, animationTime);
            joint.transform.scale = CalculateValue(rootNodeAnimation.scale, animationTime);
        }
    }
}

void Matio::Draw(WorldTransform& worldTransform, CameraRole& camera) {


    property_ = GraphicsPipeline::GetInstance()->GetPSO().Animation;

    // Rootsignatureを設定。PSOに設定してるけど別途設定が必要
    DirectXCommon::GetCommandList()->SetGraphicsRootSignature(property_.rootSignature_.Get());
    DirectXCommon::GetCommandList()->SetPipelineState(property_.graphicsPipelineState_.Get()); // PSOを設定

    DirectXCommon::GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


   // worldTransform.TransferMatrix(resource_.wvpResource, camera);
   DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(0, resource_.materialResource->GetGPUVirtualAddress());
   DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(1, worldTransform.constBuff->GetGPUVirtualAddress());
   DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(2, camera.constBuff_->GetGPUVirtualAddress());
   auto test = SrvManager::GetInstance()->GetDescriptorHeapForGPU(texHandle_);
   DirectXCommon::GetCommandList()->SetGraphicsRootDescriptorTable(3, SrvManager::GetInstance()->GetDescriptorHeapForGPU(texHandle_));

   DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(5, resource_.directionalLightResource->GetGPUVirtualAddress());

    ApplyAnimation(skeleton_, animation_,animationTime_);
    SkeletonUpdate(skeleton_);
    SkinClusterUpdate(skinCluster_, skeleton_);

    D3D12_VERTEX_BUFFER_VIEW vbvs[2] = {
        VertexBufferView_,
        skinCluster_.influenceBufferView
    };
    DirectXCommon::GetCommandList()->IASetVertexBuffers(0, 2, vbvs); // VBVを設定
    DirectXCommon::GetCommandList()->IASetIndexBuffer(&IndexBufferView_); // IBVを設定
    DirectXCommon::GetCommandList()->SetGraphicsRootDescriptorTable(4, skinCluster_.paletteSrvHandle.second);

    // 描画。(DrawCall/ドローコール)。
    DirectXCommon::GetCommandList()->DrawIndexedInstanced(UINT(modelData_.indices.size()), 1, 0, 0, 0);

}
