#include "WeaponManager.h"
#include<cassert>

void WeaponManager::Initialize(){
	//=================================================================
	//				モデルの初期化
	//=================================================================
	ModelInitialize();

	//=================================================================
	//				ハンマー
	//=================================================================
	auto hammer = std::make_unique<Hammer>();
	hammer->Initialize(weaponModels_[WeaponName::hammer].get());
	weapons_.emplace(WeaponName::hammer, std::move(hammer));
}

void WeaponManager::ModelInitialize(){
	//===================================================================
	//				ハンマー
	//===================================================================
	weaponModels_.push_back(std::unique_ptr<Model>(Model::CreateFromOBJ("hammer", true)));
}

WeaponBase* WeaponManager::GetWeapon(const WeaponName& name)const{
	const auto& it = weapons_.find(name);
	assert(it != weapons_.end());
	return it->second.get();
}