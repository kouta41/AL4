/// <summary>
/// タイトルシーン
/// </summary>
#pragma once
#include "IsScen.h"
#include "Input/Input.h"
#include "Model/Model.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "Sprite/Sprite.h"
#include "Object3D.h"
#include "Animation/Animation.h"
#include "Skybox/Skybox.h"
#include "BlockManager.h"
#include "Skydome.h"
#include "CollisionManager.h"
#include "Audio/Audio.h"
#include "BlockCore.h"
#include "GlobalVariables.h"
#include "GameObject.h"
#include "TitleObject.h"
#include "imgui.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "json.hpp"
#include <vector>
#include <string>
#include <algorithm>

class TitleScene : public IScene
{
public:

	TitleScene();
	~TitleScene();

	void Initialize()override;

	void Update()override;

	void Draw()override;

	

	// 成功回数が5位以内より高ければランキング登録
	void TryInsertToRanking(const std::string & playerName, int combo);
	//ランキングデータの読み込み
	void LoadRankingFromJson(const std::string& jsonText);
	//ランキングデータの保存
	std::string CreateRankingJson();



private:
	//キーボード入力
	Input* input_ = nullptr;

	// 成功率（%）
	 int SUCCESS_RATE = 81;

	 //継続数
	 int combo = 1;
	 //成功か失敗か
	 bool success = false;

	 //リセット
	 bool reset = false;

	 // 上位5件用
	 std::vector<RankEntry> ranking;

	//グループ名
	const char* groupName = "TitleObject";
	//ブロックの着地点
	float LandingPosition_;

	float Color_R = 1.0f;
	float Color_L = 1.0f;
	float speed = 0.06f;
};