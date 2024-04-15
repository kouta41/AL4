#include "Animation.h"
#include <omp_llvm.h>
#include <atlevent.h>

Animation   LoadAnimationFile(const std::string& directoryPath, const std::string& filename)
{
    Animation animation;//アニメーション
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
            nodeAnimation.translate.push_back(keyframe);//ここがミスってるかも
        }
    }
    return animation;
}

Vector3 CalculateValue(const std::vector<KeyframeVector3>& keyframes, float time){
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
}
//くおたにおんも作る