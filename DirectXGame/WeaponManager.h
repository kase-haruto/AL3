#pragma once
#include"Hammer.h"

#include<memory>
#include<unordered_map>
#include"Model.h"


/// <summary>
/// 武器
/// </summary>
enum WeaponName{
	hammer,
	weaponCount
};

/// <summary>
/// 武器管理クラス
/// </summary>
class WeaponManager{
public:
	/// <summary>
	/// コンストラクタ/デストラクタ
	/// </summary>
	WeaponManager() = default;
	~WeaponManager() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	WeaponBase* GetWeapon(const WeaponName& name)const;

private:
	/// <summary>
	/// モデルの初期化
	/// </summary>
	void ModelInitialize();

private:
	//各武器
	std::unordered_map<WeaponName, std::unique_ptr<WeaponBase>>weapons_;

	//各武器のモデル
	std::vector<std::unique_ptr<Model>>weaponModels_;

};

