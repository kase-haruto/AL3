#include "PlayerMowDown.h"

void PlayerMowDown::Initialize(Player* player){
	player_ = player;
	player_->SetWeaponRotation(weaponInitRotation_);
	player_->SetWeaponTranslation(weaponInitTranslation_);
}

void PlayerMowDown::Execute(Player* player){
	auto& weaponAngle = player->GetPartsTransform(int(Parts::weapon))->rotation_;

	weaponAngle = Lerp(weaponAngle, weaponTargetAngle_, 0.25f);


	// 目標角度に達したら現在の攻撃を終了
	if (std::abs(weaponTargetAngle_.x - weaponAngle.x) <= 0.001f){
		isFinished_ = true;
	}
}