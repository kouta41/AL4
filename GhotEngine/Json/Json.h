#pragma once
#include <json.hpp>
#include <fstream>
#include <map>
#include "Vector3.h"
#include "Model.h"
#include "Object3D.h"
#include "CameraRole.h"
#include "WorldTransform.h"


//レベルデータ
struct  LevelData {
	//オブジェクトの１個分のデータ
	struct ObjectData {
		std::string fileName;
		Vector3 translation;
		Vector3 rotation;
		Vector3 scaling;
	};

	//オブジェクトのコンテナ
	std::vector<ObjectData> objects;
};


class Json {
public:


	LevelData* LoadJson(const std::string& fileName);

	void Adoption(LevelData* levelData);

	void Update();

	void Draw(CameraRole camera);


	//const CameraRole& GetCamera() { return camera_; }

private:



	std::vector<std::unique_ptr<Object3DPlacer>> objects_;

	//CameraRole camera_;

	WorldTransform worldTransform_;
	uint32_t texHandle_ = 0;

};

