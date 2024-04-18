#pragma once
#include "Mathfunction.h"
#include "Model.h"
#include "WorldTransform.h"
#include "CameraRole.h"
#include <map>

template<typename tValue>
struct Keyframe {
	float time;
	tValue value;
};
using KeyframeVector3 = Keyframe<Vector3>;
using KeyframeQuaternion = Keyframe<Quaternion>;


struct NodeAnimation {
	std::vector<KeyframeVector3> translate;
	std::vector<KeyframeQuaternion> rotate;
	std::vector<KeyframeVector3> scale;
};

template<typename tValue>
struct AnimationCurve {
	std::vector<Keyframe<tValue>> keyframes;
};

//struct NodeAnimation {
//	AnimationCurve<Vector3> translate;
//	AnimationCurve<Quaternion> rotate;
//	AnimationCurve<Vector3> scale;
//};

struct Animation{
	float duration;//アニメーション全体の尺(単位は秒)
	//NodeAnimationの集合。Node名でひけるようにしておく
	std::map<std::string, NodeAnimation> nodeAnimations;
};


class Matio
{
public:
	Matio();
	~Matio();


	//アニメーションの解析
	Animation LoadAnimationFile(const std::string& directoryPath, const std::string& filename);

	//任意の時刻の値を取得する(Vecter3)ver
	Vector3 CalculateValue(const std::vector<KeyframeVector3>& keyframes, float time);

	//任意の時刻の値を取得する(Quaternion)ver
	Quaternion CalculateValue(const std::vector<KeyframeQuaternion>& keyframes, float time);


	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>

	void Playback(const WorldTransform& worldTransform, const CameraRole& camera);


#pragma region Setter

	void SetModel(const std::string& directoryPath, const std::string& filename) { animation = LoadAnimationFile(directoryPath, filename); }

	void SetAnimation(const Animation& animation_) { animation = animation_; }

	void SetModelData(const ModelData& modelData_) { modelData = modelData_; }

#pragma endregion

private:
	Animation animation;//アニメーション
	ModelData modelData;
	float animationTime = 0.0f;
	ParticleForGPU* transformData_ = nullptr;
	Resource resource_ = {};
	D3D12_VERTEX_BUFFER_VIEW AnimationVertexBufferView_{};
};

///モデルの所と統合するかもしれ

