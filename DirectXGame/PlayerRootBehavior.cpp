#include "PlayerRootBehavior.h"
#include "PlayerAttackBehavior.h"
#include "PlayerDashBehavior.h"
#include "PlayerJumpBehavior.h"

#include "Player.h"
#include <numbers>
#include "Input.h"
#include "GlobalVariables.h"
#include <cmath>
#include"LockOn.h"

void PlayerRootBehavior::ApplyGlobalVariables(){
    GlobalVariables* globalVariables = GlobalVariables::GetInstance();
    const char* groupName = "Player";
    cycle_ = globalVariables->GetValue<int32_t>(groupName, "floatingCycle");
    floatingAmplitude_ = globalVariables->GetValue<float>(groupName, "floatingAmplitude");
}

PlayerRootBehavior::PlayerRootBehavior(Player* player){
    player_ = player;
    const char* groupName = "Player";
    GlobalVariables* globalVariables = GlobalVariables::GetInstance();

    // グループを追加
    globalVariables->AddItem(groupName, "moveSpeed", speed_);
    globalVariables->AddItem(groupName, "floatingCycle", cycle_);
    globalVariables->AddItem(groupName, "floatingAmplitude", floatingAmplitude_);
}

PlayerRootBehavior::~PlayerRootBehavior(){}

void PlayerRootBehavior::Initialize(){
    player_->SetIsAttack(false);
    Vector3 L_Arm_Rotate = {0.0f, player_->GetPartsTransform(static_cast< int >(Parts::L_arm))->rotation_.y, player_->GetPartsTransform(static_cast< int >(Parts::L_arm))->rotation_.z};
    Vector3 R_Arm_Rotate = {0.0f, player_->GetPartsTransform(static_cast< int >(Parts::R_arm))->rotation_.y, player_->GetPartsTransform(static_cast< int >(Parts::R_arm))->rotation_.z};
    player_->Set_L_ArmRotation(L_Arm_Rotate);
    player_->Set_R_ArmRotation(R_Arm_Rotate);
    player_->SetBodyRotation({0.0f,0.0f,0.0f});

}

void PlayerRootBehavior::Update(){
    XINPUT_STATE padState = {};
    DWORD dwResult = XInputGetState(0, &padState);

    // 調整項目の適用
    ApplyGlobalVariables();

    if (player_->GetIsActive()){ //操作可能なら

        if (dwResult == ERROR_SUCCESS){
            if (padState.Gamepad.wButtons & XINPUT_GAMEPAD_X){
                player_->ChangeState(std::make_unique<PlayerAttackBehavior>(player_));
                return;
            }

            if (padState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER){
                player_->ChangeState(std::make_unique<PlayerDashBehavior>(player_));
                return;
            }

            // ジャンプボタンを押したら
            if (padState.Gamepad.wButtons & XINPUT_GAMEPAD_A){
                player_->ChangeState(std::make_unique<PlayerJumpBehavior>(player_));
                return;
            }
        }

        // 移動処理
        Move();

    }



    // 浮遊ギミックの処理
    UpdateFloatingAction();
}

void PlayerRootBehavior::Move(){
    if (Input::GetInstance()->PushKey(DIK_W)){
        Vector3 vel {0.0f,0.0f,player_->GetWorldPosition().z+ 0.5f};
        player_->SetPos(vel);
    }

    XINPUT_STATE joyState = {};
    if (Input::GetInstance()->GetJoystickState(0, joyState)){
        const float threshold = 0.7f;
        bool isMoving = false;

        // 移動量
        Vector3 vel = {static_cast< float >(joyState.Gamepad.sThumbLX) / SHRT_MAX, 0.0f, static_cast< float >(joyState.Gamepad.sThumbLY) / SHRT_MAX};
        player_->SetVelocity(vel);
        if (Length(vel) > threshold){
            isMoving = true;
        }

        if (isMoving){
            // 移動量に速さを反映
            player_->SetDirection(Normalize(player_->GetVelocity()));
            // 向いている方向に移動
            player_->MoveInDirection(speed_);
        }
        ///ジョイスティックによる入力がない
        else if (player_->GetLockOn() && player_->GetLockOn()->ExistTarget()){
            Vector3 lockOnTargetPos = player_->GetLockOn()->GetTargetPosition();
            Vector3 sub = lockOnTargetPos - player_->GetWorldPosition();

            //距離
            float distance = Length(sub);
            //距離しきい値
            const float moveThreshold = 0.2f;

            //しきい値より離れているときのみ
            if (distance> moveThreshold){
                Vector3 newRotate {player_->GetRotation().x,std::atan2(sub.x,sub.z),player_->GetRotation().z};
                player_->SetRotation(newRotate);

                //しきい値を超える速さなら修正
                if (speed_>distance - moveThreshold){
                    speed_ = distance - moveThreshold;
                }
            }
        }
    }
}

void PlayerRootBehavior::InitializeFloatingAction(){
    floatingParameter_ = 1.0f;
    floatingAmplitude_ = 0.45f;
}

void PlayerRootBehavior::UpdateFloatingAction(){
    // 浮遊移動のサイクル
    const float step = 2.0f * static_cast< float >(std::numbers::pi) / cycle_;
    floatingParameter_ += step;
    floatingParameter_ = std::fmod(floatingParameter_, 2.0f * static_cast< float >(std::numbers::pi));

    // 浮遊を座標に反映
    player_->SetBodyTranslationY(std::sin(floatingParameter_) * floatingAmplitude_);
}
