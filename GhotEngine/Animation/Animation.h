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
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="directoryPath"></param>
	/// <param name="filename"></param>
	/// <returns></returns>
	void Initialize(ModelData modeldata_, Animation animation_);

	//アニメーションの解析
	Animation LoadAnimationFile(const std::string& directoryPath, const std::string& filename);

	//任意の時刻の値を取得する(Vecter3)ver
	Vector3 CalculateValue(const std::vector<KeyframeVector3>& keyframes, float time);

	//任意の時刻の値を取得する(Quaternion)ver
	Quaternion CalculateValue(const std::vector<KeyframeQuaternion>& keyframes, float time);

	//Skeletonの作成
	Skeleton CreateSkeleton();

	//jointの作成
	int32_t CreateJoint(const Node& node, const std::optional<int32_t>& parent, std::vector<Joint>& joints);
	
	/// <summary>
	/// リソース作成
	/// </summary>
	/// <param name="modelData"></param>
	void CreateResource();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="skeleton"></param>
	void Update(Skeleton& skeleton);

	/// <summary>
	/// アニメーションに適用する
	/// </summary>
	/// <param name="skeleton"></param>
	/// <param name="animation"></param>
	/// <param name="animationTime"></param>
	void ApplyAnimation(Skeleton& skeleton, float animationTime);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	void Playback(WorldTransform& worldTransform, CameraRole& camera);


#pragma region Setter

	void SetModel(const std::string& directoryPath, const std::string& filename) { animation = LoadAnimationFile(directoryPath, filename); }

	void SetAnimation(const Animation& animation_) { animation = animation_; }

	void SetModelData(const ModelData& modelData_) { modelData = modelData_; }

	void SetTexHandle(uint32_t texHandle) { texHandle_ = texHandle; }


#pragma endregion

private:
	Animation animation;//アニメーション
	ModelData modelData;
	Skeleton skeleton;
	float animationTime = 0.0f;
	Resource resource_ = {};
	D3D12_VERTEX_BUFFER_VIEW AnimationVertexBufferView_{};
	uint32_t index_ = 0;
	uint32_t texHandle_ = 0;
	TransformationMatrix* transformData_ = {};

};

///モデルの所と統合するかもしれ

