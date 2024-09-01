#include "PlayerRootBehavior.h"
#include "PlayerAttackBehavior.h"
#include "PlayerDashBehavior.h"
#include "PlayerJumpBehavior.h"
#include "Player.h"
#ifdef _DEBUG
#include "GlobalVariables.h"
#endif // _DEBUG

#include "Input.h"
#include "LockOn.h"
#include <cmath>
#include <numbers>

PlayerRootBehavior::PlayerRootBehavior(Player* player)
    : PlayerBaseBehavior(player), floatingParameter_(1.0f), floatingAmplitude_(0.45f), speed_(0.3f), cycle_(70){
#ifdef _DEBUG
    GlobalVariables* globalVariables = GlobalVariables::GetInstance();
    const char* groupName = "Player";
    globalVariables->AddItem(groupName, "moveSpeed", speed_);
    globalVariables->AddItem(groupName, "floatingCycle", cycle_);
    globalVariables->AddItem(groupName, "floatingAmplitude", floatingAmplitude_);
#endif // _DEBUG
}

PlayerRootBehavior::~PlayerRootBehavior(){}

void PlayerRootBehavior::Initialize(){
    player_->SetIsAttack(false);

    Vector3 L_Arm_Rotate {0.0f, player_->GetPartsTransform(static_cast< int >(Parts::L_arm))->rotation_.y, player_->GetPartsTransform(static_cast< int >(Parts::L_arm))->rotation_.z};
    Vector3 R_Arm_Rotate {0.0f, player_->GetPartsTransform(static_cast< int >(Parts::R_arm))->rotation_.y, player_->GetPartsTransform(static_cast< int >(Parts::R_arm))->rotation_.z};
    player_->Set_L_ArmRotation(L_Arm_Rotate);
    player_->Set_R_ArmRotation(R_Arm_Rotate);
    player_->SetBodyRotation({0.0f, 0.0f, 0.0f});

#ifdef _DEBUG
    ApplyGlobalVariables();
#endif // _DEBUG
}

void PlayerRootBehavior::Update(){
    XINPUT_STATE padState = {};
    DWORD dwResult = XInputGetState(0, &padState);

    if (player_->GetIsActive()){
        if (dwResult == ERROR_SUCCESS){
            if (padState.Gamepad.wButtons & XINPUT_GAMEPAD_X){
                player_->ChangeState(std::make_unique<PlayerAttackBehavior>(player_));
                return;
            }
            if (padState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER){
                player_->ChangeState(std::make_unique<PlayerDashBehavior>(player_));
                return;
            }
            if (padState.Gamepad.wButtons & XINPUT_GAMEPAD_A){
                player_->ChangeState(std::make_unique<PlayerJumpBehavior>(player_));
                return;
            }
        }
        Move();
    }

    UpdateFloatingAction();
}

void PlayerRootBehavior::Move(){
    XINPUT_STATE joyState = {};
    if (Input::GetInstance()->GetJoystickState(0, joyState)){
        const float threshold = 0.7f;
        Vector3 vel = {static_cast< float >(joyState.Gamepad.sThumbLX) / SHRT_MAX, 0.0f, static_cast< float >(joyState.Gamepad.sThumbLY) / SHRT_MAX};
        player_->SetVelocity(vel);

        if (Length(vel) > threshold){
            player_->SetDirection(Normalize(player_->GetVelocity()));
            player_->MoveInDirection(speed_);
        } else if (player_->GetLockOn() && player_->GetLockOn()->ExistTarget()){
            HandleLockOnMovement();
        }
    }
}

void PlayerRootBehavior::HandleLockOnMovement(){
    Vector3 lockOnTargetPos = player_->GetLockOn()->GetTargetPosition();
    Vector3 sub = lockOnTargetPos - player_->GetWorldPosition();

    // 敵に向かって回転する
    Vector3 newRotate {player_->GetRotation().x, std::atan2(sub.x, sub.z), player_->GetRotation().z};
    player_->SetRotation(newRotate);
}

void PlayerRootBehavior::UpdateFloatingAction(){
    const float step = 2.0f * static_cast< float >(std::numbers::pi) / cycle_;
    floatingParameter_ += step;
    floatingParameter_ = std::fmod(floatingParameter_, 2.0f * static_cast< float >(std::numbers::pi));

    player_->SetTranslationY(std::sin(floatingParameter_) * floatingAmplitude_);
}

void PlayerRootBehavior::ApplyGlobalVariables(){
#ifdef _DEBUG
    //GlobalVariables* globalVariables = GlobalVariables::GetInstance();
    //const char* groupName = "Player";
    //speed_ = globalVariables->GetValue<float>(groupName, "moveSpeed");
    //cycle_ = globalVariables->GetValue<float>(groupName, "floatingCycle");
    //floatingAmplitude_ = globalVariables->GetValue<float>(groupName, "floatingAmplitude");
#endif // _DEBUG
}
