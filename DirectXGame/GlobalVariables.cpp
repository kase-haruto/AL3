#include "GlobalVariables.h"


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

		if (!ImGui::BeginMenu(groupName.c_str())){continue;}
			//各項目について
			for (std::map<std::string, Item>::iterator itItem = group.items.begin();
				 itItem != group.items.end();++itItem){
				
				//各項目を取得
				const std::string& itemName = itItem->first;
				//項目の参照を取得
				Item& item = itItem->second;

				std::visit([&] (auto& value){
					ShowSlider(itemName, value);
						   }, item.value);
			}
		ImGui::EndMenu();
	}

	ImGui::EndMenuBar();
	ImGui::End();
}

void GlobalVariables::CreateGroup(const std::string& groupName){
	datas_[groupName];
}