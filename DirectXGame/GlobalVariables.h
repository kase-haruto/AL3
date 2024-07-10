#pragma once
#include"Vector3.h"
#include<imgui.h>
#include<map>
#include<string>
#include<variant>
#include<stdint.h>
/// <summary>
/// グローバル変数
/// </summary>
class GlobalVariables{

	/// <summary>
	/// 項目
	/// </summary>
	struct Item{
		//項目の値
		std::variant<int32_t, float, Vector3>value;
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

	template<typename T>
	void SetValue(const std::string& groupName, const std::string& key, const T& value);

private:
	GlobalVariables() = default;
	~GlobalVariables() = default;
	GlobalVariables(const GlobalVariables&) = delete;
	const GlobalVariables& operator=(const GlobalVariables&) = delete;

private:

	//全データ
	std::map<std::string, Group>datas_;
};

// テンプレート関数の実装
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
inline void ShowSlider(const std::string& itemName, T& value);

template<>
inline void ShowSlider<int32_t>(const std::string& itemName, int32_t& value){
	ImGui::SliderInt(itemName.c_str(), &value, 0, 100);
}

template<>
inline void ShowSlider<float>(const std::string& itemName, float& value){
	ImGui::SliderFloat(itemName.c_str(), &value, 0, 100.0f);
}

template<>
inline void ShowSlider<Vector3>(const std::string& itemName, Vector3& value){
	ImGui::SliderFloat3(itemName.c_str(), &value.x, -10.0f, 10.0f);
}

