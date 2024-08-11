#include "PlayerMowDown.h"

void PlayerMowDown::Initialize(Player* player){
	player->SetWeaponRotation(weaponInitRotation_);
	player->SetWeaponTranslation(weaponInitTranslation_);
	player->SetBodyRotation(bodyInitRotation);
	player->Set_L_ArmRotationX(shakeUpAngle_);
	player->Set_R_ArmRotationX(shakeUpAngle_);

	//ハンマーの接触履歴を削除
	player->GetWeapon()->ClearContactRecord();
}

void PlayerMowDown::Execute(Player* player){
	auto& bodyAngle = player->GetPartsTransform(int(Parts::body))->rotation_;
	Vector3 weaponRotate = player->GetWeaponTransform().rotation_;
	bodyAngle = Lerp(bodyAngle, bodyTargetRotation, movingStep_);
	weaponRotate = Lerp(weaponRotate, weaponTargetAngle_, movingStep_);
	player->SetWeaponRotation(weaponRotate);
	// 目標角度に達したら現在の攻撃を終了
	if (std::abs(bodyTargetRotation.y - bodyAngle.y) <= 0.01f){
		isFinished_ = true;
	}
}