/// <summary>
/// 調整項目の入出力
/// </summary>
/// 
#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>
#include <variant>
#include <string>
#include <map>
#include <Vector3.h>
#include <Vector4.h>
#include "ImGuiManager.h"
#include "imgui.h"
#include "json.hpp"


//項目
struct Item
{
	//項目の値
	std::variant<int32_t, float, Vector3, Vector4> value;
};

//グループ
struct Group
{
	std::map<std::string, Item> items;
};


class GlobalVariables{

public:
	//新ブルトンのインスタンスを取得する関数
	static GlobalVariables* GatInstance();

	/// <summary>
	/// グループの作成
	/// </summary>
	/// <param name="groypName">グループ名</param>
	void CreateGroup(const std::string& groupName);

	//値のセット(int)

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();


	/// <summary>
	/// ファイルの書き出し
	/// </summary>
	/// <param name="groupName">グループ</param>
	void SaveFile(const std::string& groupName);


	/// <summary>
	/// デレクトリの全ファイルの読み込み
	/// </summary>
	void LoadFiles();


	/// <summary>
	/// ファイルから読み込む
	/// </summary>
	/// <param name="groupName">グループ</param>
	void LoadFile(const std::string& groupName);


	//項目追加(int)
	void AddItem(const std::string& groupName, const std::string& key, int32_t value);
	//項目追加(float)
	void AddItem(const std::string& groupName, const std::string& key, float value);
	//項目追加(Vector3)
	void AddItem(const std::string& groupName, const std::string& key, const Vector3& value);
	//項目追加(Vector4)
	void AddItem(const std::string& groupName, const std::string& key, const Vector4& value);

	/// <summary>
	/// Setter
	/// </summary>
	/// <param name="groupName">グループ</param>
	/// <param name="key">キー</param>
	/// <param name="value">値</param>
	//値のセット(int) 
	void SetValue(const std::string& groupName, const std::string& key, int32_t value);
	//値のセット(float)
	void SetValue(const std::string& groupName, const std::string& key, float value);
	//値のセット(Vector3)
	void SetValue(const std::string& groupName, const std::string& key, const Vector3& value);
	//値のセット(Vector4)
	void SetValue(const std::string& groupName, const std::string& key, const Vector4& value);


	/// <summary>
	/// Getter
	/// </summary>
	/// <param name="groupName">グループ</param>
	/// <param name="key">キー</param>
	/// <returns></returns>
	//値を渡す(int) 
	int32_t GetIntValue(const std::string& groupName, const std::string& key)const;
	//値を渡す(float) 
	float GetFloatValue(const std::string& groupName, const std::string& key)const;
	//値を渡す(Vectot3) 
	Vector3 GetVector3Value(const std::string& groupName, const std::string& key)const;
	//値を渡す(Vector4) 
	Vector4 GetVector4Value(const std::string& groupName, const std::string& key)const;

private:
	//コンストラクタ
	GlobalVariables() = default;

	//デストラクタ
	~GlobalVariables() = default;

	//コピーコンストラクタの禁止
	GlobalVariables(const GlobalVariables&) = default;

	//コピー代入演算子の禁止
	GlobalVariables& operator=(const GlobalVariables&) = default;

	using json = nlohmann::json;

	//全データ
	std::map<std::string, Group> datas_;

	//グローバル変数の保持先ファイルパス
	const std::string kDirectoryPath = "resources/GlobalVariables/";

};

