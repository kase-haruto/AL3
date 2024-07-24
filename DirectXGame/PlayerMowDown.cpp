#include "PlayerMowDown.h"

void PlayerMowDown::Initialize(Player* player){
	player_ = player;
	player_->SetWeaponRotation(weaponInitRotation_);
	player_->SetWeaponTranslation(weaponInitTranslation_);
	player_->SetBodyRotation(bodyInitRotation);
	player_->Set_L_ArmRotationX(shakeUpAngle_);
	player_->Set_R_ArmRotationX(shakeUpAngle_);
}

void PlayerMowDown::Execute(Player* player){
	auto& bodyAngle = player->GetPartsTransform(int(Parts::body))->rotation_;

	bodyAngle = Lerp(bodyAngle, bodyTargetRotation, movingStep_);


	// 目標角度に達したら現在の攻撃を終了
	if (std::abs(bodyTargetRotation.y - bodyAngle.y) <= 0.01f){
		isFinished_ = true;
	}
}