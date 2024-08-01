#include "PlayerWeaponSwingDown.h"

void PlayerWeaponSwingDown::Initialize(Player* player){
	player_ = player;
	player_->SetWeaponRotation(weaponInitAngle_);
	player_->Set_L_ArmRotationX(shakeUpAngle_);
	player_->Set_R_ArmRotationX(shakeUpAngle_);
	Vector3 init = {0.0f,0.0f,0.0f};
	player_->SetBodyRotation(init);
	isFinished_ = false;
}

void PlayerWeaponSwingDown::Execute(Player* player){
	Vector3 weaponAngle = player->GetWeaponTransform().rotation_;
	auto& L_armAngle = player->GetPartsTransform(int(Parts::L_arm))->rotation_;
	auto& R_armAngle = player->GetPartsTransform(int(Parts::R_arm))->rotation_;

	weaponAngle.x = Lerp(weaponAngle.x, targetArmAngle_, movingStep_);
	L_armAngle.x = Lerp(L_armAngle.x, -targetArmAngle_, movingStep_);
	R_armAngle.x = Lerp(R_armAngle.x, -targetArmAngle_, movingStep_);


	// 目標角度に達したら現在の攻撃を終了
	if (std::abs(targetArmAngle_ - L_armAngle.x) <= 0.01f){
		isFinished_ = true;
	}
}