#pragma once
#include "Mathfunction.h"
#include "WorldTransform.h"
#include "CameraRole.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "GraphicsPipeline.h"
#include <DescriptorManager/SrvManager/SrvManager.h>
#include <map>
#include <optional>
#include <span>
#include <numbers>
#include <map>
#include <fstream>
#include <sstream>
#include "IModelState.h"
#include "ModelSphere.h"
#include "Vector2.h"
#include "ImGuiManager.h"




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

struct Animation {
	float duration;//アニメーション全体の尺(単位は秒)
	//NodeAnimationの集合。Node名でひけるようにしておく
	std::map<std::string, NodeAnimation> nodeAnimations;
};

class Motion
{
public:
	Motion();
	~Motion();
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="directoryPath"></param>
	/// <param name="filename"></param>
	/// <returns></returns>
	void Initialize(const std::string& fileName);

	//アニメーションの解析
	Animation LoadAnimationFile(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// アニメーションのObjファイルを読む
	ModelData LoadAnimationObjFile(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// アニメーションのGLTFファイルを読む
	ModelData LoadAnimationGLTFFile(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// ノードを読む
	Node ReadNode(aiNode* node);


	//任意の時刻の値を取得する(Vecter3)ver
	Vector3 CalculateValue(const std::vector<KeyframeVector3>& keyframes, float time);

	//任意の時刻の値を取得する(Quaternion)ver
	Quaternion CalculateValue(const std::vector<KeyframeQuaternion>& keyframes, float time);

	//Skeletonの作成
	Skeleton CreateSkeleton(const Node& rootNode);

	//jointの作成
	int32_t CreateJoint(const Node& node, const std::optional<int32_t>& parent, std::vector<Joint>& joints);
	
	//SkinClusterの作成
	SkinCluster CreateSkinCluster(ModelData& modelData,const Skeleton& skeleton);
	/// <summary>
	/// リソース作成
	/// </summary>
	/// <param name="modelData"></param>
	void CreateResource();

	/// <summary>
	/// Skeletonの更新
	/// </summary>
	/// <param name="skeleton"></param>
	void SkeletonUpdate(Skeleton& skeleton);

	/// <summary>
	/// SkinClusterの更新
	/// </summary>
	/// <param name="skinCluster"></param>
	void SkinClusterUpdate(SkinCluster& skinCluster,const Skeleton& skeleton);

	/// <summary>
	/// アニメーションに適用する
	/// </summary>
	/// <param name="skeleton"></param>
	/// <param name="animation"></param>
	/// <param name="animationTime"></param>
	void ApplyAnimation(Skeleton& skeleton, const Animation& animation, float animationTime);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	void Draw(WorldTransform& worldTransform, CameraRole& camera);


#pragma region Setter

	void SetModel(const std::string& directoryPath, const std::string& filename) { animation_= LoadAnimationFile(directoryPath, filename); }

	void SetAnimation(const Animation& animation) { animation_= animation; }

	void SetModelData(const ModelData& modelData) { modelData_ = modelData; }

	void SetTexHandle(uint32_t texHandle) { texHandle_ = texHandle; }

	void SetanimationTime(float animationTime) { animationTime_ = animationTime; }

#pragma endregion

private:
	Animation animation_;//アニメーション
	ModelData modelData_;
	Skeleton skeleton_;
	float animationTime_ = 0.0f;
	Resource resource_ = {};
	D3D12_VERTEX_BUFFER_VIEW VertexBufferView_{};
	D3D12_INDEX_BUFFER_VIEW IndexBufferView_{};
	uint32_t index_ = 0;
	uint32_t texHandle_ = 0;
	uint32_t srvIndex_ = 0;
	SkinCluster skinCluster_;
	Property property_{};
	Material* materialData_ = nullptr;
	Vector4 color_ = {};
	DirectionalLight* directionalLightData_ = nullptr;

};

///モデルの所と統合するかもしれ

