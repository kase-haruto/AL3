#include "PlayerWeaponSwingDown.h"

void PlayerWeaponSwingDown::Initialize(Player* player){
	player->SetIsAttack(true);
	player->SetWeaponRotation(weaponInitAngle_);
	player->Set_L_ArmRotationX(shakeUpAngle_);
	player->Set_R_ArmRotationX(shakeUpAngle_);
	isFinished_ = false;
}

void PlayerWeaponSwingDown::Execute(Player* player){
	auto& weaponAngle = player->GetPartsTransform(int(Parts::weapon))->rotation_;
	auto& L_armAngle = player->GetPartsTransform(int(Parts::L_arm))->rotation_;
	auto& R_armAngle = player->GetPartsTransform(int(Parts::R_arm))->rotation_;

	weaponAngle.x = Lerp(weaponAngle.x, targetArmAngle_, 0.25f);
	L_armAngle.x = Lerp(L_armAngle.x, -targetArmAngle_, 0.25f);
	R_armAngle.x = Lerp(R_armAngle.x, -targetArmAngle_, 0.25f);


	// 目標角度に達したら現在の攻撃を終了
	if (std::abs(targetArmAngle_ - weaponAngle.x) <= 0.001f){
		isFinished_ = true;
	}
}