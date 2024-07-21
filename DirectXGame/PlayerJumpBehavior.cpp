#include "PlayerJumpBehavior.h"
#include"Player.h"
#include"GlobalVariables.h"

PlayerJumpBehavior::PlayerJumpBehavior(Player* player){
	player_ = player;

	
	const char* groupName = "Player";
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	globalVariables->CreateGroup(groupName);

	//グループを追加
	globalVariables->AddItem(groupName, "GravityAcceleration", kGravityAcceleration_);
	globalVariables->AddItem(groupName, "jumpFirstSpeed", jumpFirstSpeed_);
}

PlayerJumpBehavior::~PlayerJumpBehavior(){}

void PlayerJumpBehavior::Initialize(){
	player_->SetBodyRotationY(0.0f);
	player_->Set_L_ArmRotationX(0.0f);
	player_->Set_L_ArmRotationX(0.0f);

	
	//ジャンプ初速を与える
	player_->SetVelocity(jumpFirstSpeed_);
}

void PlayerJumpBehavior::Update(){
	//移動
	Vector3 translation = player_->GetWorldTransform().translation_;
	Vector3 vel = player_->GetVelocity();
	player_->SetTranslation(translation + vel);
	
	//加速度ベクトル
	Vector3 accelerationVector = {0,-kGravityAcceleration_,0};
	//加速する
	player_->SetVelocity(vel + accelerationVector);

	if (translation.y < 0.0f){
		translation.y = 0.0f;
		//ジャンプ終了
		player_->SetBehavior(Behavior::root);
	}
}

void PlayerJumpBehavior::ApplyGlobalVariables(){
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	kGravityAcceleration_ = globalVariables->GetValue<float>(groupName, "GravityAcceleration");
	jumpFirstSpeed_ = globalVariables->GetValue<Vector3>(groupName, "jumpFirstSpeed");

}