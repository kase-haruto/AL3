#include "GlobalVariables.h"
#include<fstream>
#include"WinApp.h"

GlobalVariables* GlobalVariables::GetInstance(){
	static GlobalVariables instance;
	return &instance;
}

void GlobalVariables::Update(){
	if (!ImGui::Begin("Global Variables", nullptr, ImGuiWindowFlags_MenuBar)){
		ImGui::End();
		return;
	}
	if (!ImGui::BeginMenuBar()){ return; }

	//各グループについて
	for (std::map<std::string, Group>::iterator itGroup = datas_.begin();
		 itGroup != datas_.end(); ++itGroup){
		//グループ名を取得
		const std::string& groupName = itGroup->first;
		//グループの参照を取得
		Group& group = itGroup->second;

		if (!ImGui::BeginMenu(groupName.c_str())){ continue; }
		//各項目について
		for (std::map<std::string, Item>::iterator itItem = group.items.begin();
			 itItem != group.items.end(); ++itItem){

			//各項目を取得
			const std::string& itemName = itItem->first;
			//項目の参照を取得
			Item& item = itItem->second;

			std::visit([&] (auto& value){
				ShowSlider(itemName, value);
					   }, item.value);
		}

		ImGui::Text("\n");

		if (ImGui::Button("save")){
			SaveFile(groupName);
			std::string message = std::format("{}.json saved.", groupName);
			MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
		}

		ImGui::EndMenu();
	}

	ImGui::EndMenuBar();
	ImGui::End();
}

void GlobalVariables::CreateGroup(const std::string& groupName){
	datas_[groupName];
}

void GlobalVariables::SaveFile(const std::string& groupName){
	//グループを検索
	std::map<std::string, Group>::iterator itGroup = datas_.find(groupName);
	//未登録チェック
	assert(itGroup != datas_.end());

	json root;

	root = json::object();
	//jsonオブジェクト
	root[groupName] = json::object();

	//各項目について
	for (std::map<std::string, Item>::iterator itItem = itGroup->second.items.begin();
		 itItem != itGroup->second.items.end(); ++itItem){
		//項目名を取得
		const std::string& itemName = itItem->first;
		//項目の参照を取得
		Item& item = itItem->second;

		//int32_t型の値を保持している場合
		if (std::holds_alternative<int32_t>(item.value)){
			//int32_t型の値を登録
			root[groupName][itemName] = std::get<int32_t>(item.value);
		}

		//float型の値を保持している場合
		else if (std::holds_alternative<float>(item.value)){
			//float型の値を登録
			root[groupName][itemName] = std::get<float>(item.value);
		}

		//vector3型の値を保持している場合
		else if (std::holds_alternative<Vector3>(item.value)){
			//float型のjson配列登録
			Vector3 value = std::get<Vector3>(item.value);
			root[groupName][itemName] = json::array({value.x,value.y,value.z});
		}
	}

	//ディレクトリがなければ作成する
	std::filesystem::path dir(kDirectoryPath);
	if (!std::filesystem::exists(kDirectoryPath)){
		std::filesystem::create_directory(kDirectoryPath);
	}

	//書き込むjsonファイルの古パスを合成する
	std::string filePath = kDirectoryPath + groupName + ".json";
	//書き込むようファイルストリーム
	std::ofstream ofs;
	//ファイルを書き込みように開く
	ofs.open(filePath);

	//エラーハンドリング
	if (ofs.fail()){
		std::string message = "Failed open data file for write.";
		MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
		assert(0);
		return;
	}

	//ファイルにjson文字列を書き込む
	ofs << std::setw(4) << root << std::endl;
	//ファイルを閉じる
	ofs.close();
}

void GlobalVariables::LoadFiles(){
	//ディレクトリがなければスキップする
	std::filesystem::path dir(kDirectoryPath);
	if (!std::filesystem::exists(kDirectoryPath)){
		return;
	}

	std::filesystem::directory_iterator dir_it(kDirectoryPath);
	for (const std::filesystem::directory_entry& entry : dir_it){
		//ファイルパスを取得
		const std::filesystem::path& filePath = entry.path();

		//ファイル拡張子を取得
		std::string extension = filePath.extension().string();
		//.jsonファイル以外はスキップ
		if (extension.compare(".json") != 0){
			continue;
		}

		//ファイル読み込み
		LoadFile(filePath.stem().string());

	}
}

void GlobalVariables::LoadFile(const std::string& groupName){
	// 読み込むjsonファイルのパスを合成する
	std::string filePath = kDirectoryPath + groupName + ".json";
	// 読み込み用ファイルストリーム
	std::ifstream ifs(filePath);

	// エラーハンドリング
	if (ifs.fail()){
		std::string message = "Failed to open data file for read.";
		MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
		assert(0);
		return;
	}

	json root;

	// json文字列からjsonのデータ構造に展開
	ifs >> root;
	// ファイルを閉じる
	ifs.close();

	// グループを検索
	auto itGroup = root.find(groupName);
	// 未登録チェック
	assert(itGroup != root.end());

	// 各アイテムについて
	for (auto itItem = itGroup->begin(); itItem != itGroup->end(); ++itItem){
		// アイテム名を取得
		const std::string& itemName = itItem.key();

		// int32_t型の値を保持していれば
		if (itItem->is_number_integer()){
			// int型の値を登録
			int32_t value = itItem->get<int32_t>();
			SetValue(groupName, itemName, value);
		}
		// float型の値を保持している場合
		else if (itItem->is_number_float()){
			// float型の値
			float value = itItem->get<float>();
			SetValue(groupName, itemName, value);
		}
		// 要素数3の配列であれば
		else if (itItem->is_array() && itItem->size() == 3){
			// Vector3型のjson配列を登録
			Vector3 value = {itItem->at(0), itItem->at(1), itItem->at(2)};
			SetValue(groupName, itemName, value);
		}
	}
}

