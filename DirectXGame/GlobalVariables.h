#pragma once
#include"Vector3.h"
#include<imgui.h>
#include<map>
#include<string>
#include<variant>
#include<stdint.h>
#include<json.hpp>

using json = nlohmann::json;

/// <summary>
/// グローバル変数
/// </summary>
class GlobalVariables{

	/// <summary>
	/// 項目
	/// </summary>
	struct Item{
		//項目の値
		std::variant<int32_t, float, Vector3,bool>value;
	};

	/// <summary>
	/// グループ
	/// </summary>
	struct Group{
		std::map<std::string, Item>items;
	};

public:
	static GlobalVariables* GetInstance();

	/// <summary>
	/// グループの作成
	/// </summary>
	/// <param name="groupName"></param>
	void CreateGroup(const std::string& groupName);
	/// <summary>
	/// 更新
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="groupName"></param>
	/// <param name="key"></param>
	/// <param name="value"></param>
	void Update();
	/// <summary>
	/// グループの保存
	/// </summary>
	/// <param name="groupName"></param>
	void SaveFile(const std::string& groupName);
	/// <summary>
	/// ディレクトリの全ファイル読み込み
	/// </summary>
	void LoadFiles();
	/// <summary>
	/// ファイルから読み込み
	/// </summary>
	/// <param name="groupName"></param>
	void LoadFile(const std::string& groupName);
	/// <summary>
	/// 値をセットする
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="groupName"></param>
	/// <param name="key"></param>
	/// <param name="value"></param>
	template<typename T>
	void SetValue(const std::string& groupName, const std::string& key, const T& value);
	/// <summary>
	/// 項目の追加 int/float/Vector3
	/// </summary>
	/// <param name="groupName"></param>
	/// <param name="key"></param>
	/// <param name="value"></param>
	template<typename T>
	void AddItem(const std::string& groupName, const std::string& key, const T& value);

	template<typename T>
	T GetValue(const std::string& groupName, const std::string& key)const;

	void ShowSlider(const std::string& itemName, float& value);
	void ShowSlider(const std::string& itemName, int32_t& value);
	void ShowSlider(const std::string& itemName, Vector3& value);
	void CheckBox(const std::string& itemName, bool& value);

private:
	GlobalVariables() = default;
	~GlobalVariables() = default;
	GlobalVariables(const GlobalVariables&) = delete;
	const GlobalVariables& operator=(const GlobalVariables&) = delete;

private:

	//全データ
	std::map<std::string, Group>datas_;

	//グローバル変数の保存先ファイルパス
	const std::string kDirectoryPath = "Resources/GlobalVariables/";
};

///=======================================================================================================
///		テンプレート関数の設定
///=======================================================================================================
template<typename T>
void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, const T& value){
	// グループの参照を取得
	Group& group = datas_[groupName];
	// 新しい項目のデータを設定
	Item newItem {};
	newItem.value = value;
	// 設定した項目をstd::mapに追加
	group.items[key] = newItem;
}

template<typename T>
void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, const T& value){
	// グループの参照を取得。存在しない場合は新しいグループを作成
	Group& group = datas_[groupName];
	// アイテムを検索
	auto itItem = group.items.find(key);
	// アイテムが存在しない場合のみ追加
	if (itItem == group.items.end()){
		SetValue(groupName, key, value);
	}
}

template<typename T>
T GlobalVariables::GetValue(const std::string& groupName, const std::string& key)const{
	// グループが存在することを確認
	assert(datas_.find(groupName) != datas_.end());
	// グループの参照を取得
	const Group& group = datas_.at(groupName);
	// キーが存在することを確認
	assert(group.items.find(key) != group.items.end());
	// 指定されたキーの値を取得
	return std::get<T>(group.items.at(key).value);
}





