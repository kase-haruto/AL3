#include "PlayerWeaponSwingDown.h"

void PlayerWeaponSwingDown::Initialize(Player* player){
	player->SetWeaponRotation(weaponInitAngle_);
	player->Set_L_ArmRotationX(shakeUpAngle_);
	player->Set_R_ArmRotationX(shakeUpAngle_);
	Vector3 init = {0.0f,0.0f,0.0f};
	player->SetBodyRotation(init);
	isFinished_ = false;
}

void PlayerWeaponSwingDown::Execute(Player* player){
	Vector3 weaponRotate = player->GetWeaponTransform().rotation_;

	auto& L_armAngle = player->GetPartsTransform(int(Parts::L_arm))->rotation_;
	auto& R_armAngle = player->GetPartsTransform(int(Parts::R_arm))->rotation_;

	L_armAngle.x = Lerp(L_armAngle.x, -targetArmAngle_, movingStep_);
	R_armAngle.x = Lerp(R_armAngle.x, -targetArmAngle_, movingStep_);

	weaponRotate = Lerp(weaponRotate, weaponTargetAngle_, movingStep_);
	player->SetWeaponRotation(weaponRotate);


	// 目標角度に達したら現在の攻撃を終了
	if (std::abs(weaponRotate.x - weaponTargetAngle_.x) <= 0.01f){
		isFinished_ = true;
	}
}