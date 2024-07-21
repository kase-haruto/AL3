#include "PlayerAttackBehavior.h"
#include"Player.h"
#include"GlobalVariables.h"

PlayerAttackBehavior::PlayerAttackBehavior(Player*player){
	player_ = player;

	const char* groupName = "Player";
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();

	//グループを追加
	globalVariables->AddItem(groupName, "targetArmAngle", targetArmAngle_);
	globalVariables->AddItem(groupName, "shakeUpAngle", shakeUpAngle_);
	globalVariables->AddItem(groupName, "weaponInitAngle", weaponInitAngle_);
}

PlayerAttackBehavior::~PlayerAttackBehavior(){}

void PlayerAttackBehavior::Initialize(){
	player_->SetIsAttack(true);
	
	player_->SetWeaponRotationX(weaponInitAngle_);
	player_->Set_L_ArmRotationX(shakeUpAngle_);
	player_->Set_R_ArmRotationX(shakeUpAngle_);
}

void PlayerAttackBehavior::Update(){
	auto& weaponAngle = player_->GetPartsTransform(int(Parts::weapon))->rotation_;
	auto& L_armAngle = player_->GetPartsTransform(int(Parts::L_arm))->rotation_;
	auto& R_armAngle = player_->GetPartsTransform(int(Parts::R_arm))->rotation_;

	weaponAngle.x = Lerp(weaponAngle.x, targetArmAngle_, 0.2f);
	L_armAngle.x = Lerp(L_armAngle.x, -targetArmAngle_, 0.2f);
	R_armAngle.x = Lerp(R_armAngle.x, -targetArmAngle_, 0.2f);


	// 目標角度に達したら攻撃を初期化
	if (std::abs(targetArmAngle_ - weaponAngle.x) <= 0.001f){
		player_->SetBehavior(Behavior::root);
	}
}

void PlayerAttackBehavior::ApplyGlobalVariables(){
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	targetArmAngle_ = globalVariables->GetValue<float>(groupName, "targetArmAngle");
	shakeUpAngle_ = globalVariables->GetValue<float>(groupName, "shakeUpAngle");
	weaponInitAngle_ = globalVariables->GetValue<float>(groupName, "weaponInitAngle");
}