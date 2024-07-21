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
	// 現在の位置と速度を取得
	Vector3 translation = player_->GetWorldTransform().translation_;
	Vector3 vel = player_->GetVelocity();

	// 移動処理：現在の位置に速度を加算
	player_->SetTranslation(translation + vel);

	// 加速度ベクトルを作成（重力を下方向に適用）
	Vector3 accelerationVector = {0, -kGravityAcceleration_, 0};

	// 現在の速度に加速度を加算（重力による加速）
	player_->SetVelocity(vel + accelerationVector);

	// プレイヤーが地面に到達した場合
	if (player_->GetWorldTransform().translation_.y < 0.0f){
		// y座標を0に固定（地面に到達）
		Vector3 updatedTranslation = player_->GetWorldTransform().translation_;
		updatedTranslation.y = 0.0f;
		player_->SetTranslation(updatedTranslation);

		// ジャンプを終了し、通常の行動に戻す
		player_->SetBehavior(Behavior::root);
	}
}

void PlayerJumpBehavior::ApplyGlobalVariables(){
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	kGravityAcceleration_ = globalVariables->GetValue<float>(groupName, "GravityAcceleration");
	jumpFirstSpeed_ = globalVariables->GetValue<Vector3>(groupName, "jumpFirstSpeed");

}