#include "PlayerJumpBehavior.h"
#include "PlayerRootBehavior.h"
#include "Player.h"

#ifdef _DEBUG
#include "GlobalVariables.h"
#endif // _DEBUG

PlayerJumpBehavior::PlayerJumpBehavior(Player* player) : PlayerBaseBehavior(player){
#ifdef _DEBUG
    const char* groupName = "Player";
    GlobalVariables* globalVariables = GlobalVariables::GetInstance();
    globalVariables->CreateGroup(groupName);
    globalVariables->AddItem(groupName, "GravityAcceleration", kGravityAcceleration_);
    globalVariables->AddItem(groupName, "jumpFirstSpeed", jumpFirstSpeed_);
#endif // _DEBUG
}

PlayerJumpBehavior::~PlayerJumpBehavior(){}

void PlayerJumpBehavior::Initialize(){
    player_->SetBodyRotationY(0.0f);
    player_->Set_L_ArmRotationX(0.0f);
    player_->Set_R_ArmRotationX(0.0f);

    player_->SetVelocity(jumpFirstSpeed_);
}

void PlayerJumpBehavior::Update(){
    Vector3 translation = player_->GetWorldTransform().translation_;
    Vector3 vel = player_->GetVelocity();

    player_->SetTranslation(translation + vel);

    Vector3 accelerationVector = {0, -kGravityAcceleration_, 0};
    player_->SetVelocity(vel + accelerationVector);

    if (player_->GetWorldTransform().translation_.y < 0.0f){
        Vector3 updatedTranslation = player_->GetWorldTransform().translation_;
        updatedTranslation.y = 0.0f;
        player_->SetTranslation(updatedTranslation);

        player_->ChangeState(std::make_unique<PlayerRootBehavior>(player_));
    }
}

void PlayerJumpBehavior::ApplyGlobalVariables(){
#ifdef _DEBUG
    GlobalVariables* globalVariables = GlobalVariables::GetInstance();
    const char* groupName = "Player";
    kGravityAcceleration_ = globalVariables->GetValue<float>(groupName, "GravityAcceleration");
    jumpFirstSpeed_ = globalVariables->GetValue<Vector3>(groupName, "jumpFirstSpeed");
#endif // _DEBUG
}
