#include "PlayerAttackBehavior.h"
#include "PlayerRootBehavior.h"
#include "Player.h"
#include "GlobalVariables.h"
#include "PlayerWeaponSwingDown.h"
#include "PlayerMowDown.h"
#include "LockOn.h"
#include <Xinput.h>

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

PlayerAttackBehavior::PlayerAttackBehavior(Player* player) : PlayerBaseBehavior(player), currentCommandIndex_(0){
    // コンボの追加
    AddCommand(std::make_unique<PlayerMowDown>());
    AddCommand(std::make_unique<PlayerWeaponSwingDown>());
}

PlayerAttackBehavior::~PlayerAttackBehavior(){}

void PlayerAttackBehavior::Initialize(){
    if (!attackCommands_.empty() && currentCommandIndex_ < attackCommands_.size()){
        attackCommands_[currentCommandIndex_]->Initialize(player_);
    }
    player_->SetIsAttack(true);
}

void PlayerAttackBehavior::Update(){
    XINPUT_STATE padState = {};
    XInputGetState(0, &padState);

    if (player_->GetLockOn() && player_->GetLockOn()->ExistTarget()){
        Vector3 lockOnTargetPos = player_->GetLockOn()->GetTargetPosition();
        Vector3 sub = lockOnTargetPos - player_->GetWorldPosition();

        Vector3 newRotate {player_->GetRotation().x, std::atan2(sub.x, sub.z), player_->GetRotation().z};
        player_->SetRotation(newRotate);
    }

    if (currentCommandIndex_ < attackCommands_.size()){
        attackCommands_[currentCommandIndex_]->Execute(player_);

        if (attackCommands_[currentCommandIndex_]->GetIsFinished()){
            if (--comboReceptionTime_ <= 0){
                player_->ChangeState(std::make_unique<PlayerRootBehavior>(player_));
            } else if (padState.Gamepad.wButtons & XINPUT_GAMEPAD_X){
                currentCommandIndex_++;
                if (currentCommandIndex_ < attackCommands_.size()){
                    attackCommands_[currentCommandIndex_]->Initialize(player_);
                    comboReceptionTime_ = comboReceptionTimeMax_;
                }
            }
        }
    } else{
        player_->ChangeState(std::make_unique<PlayerRootBehavior>(player_));
    }
}

void PlayerAttackBehavior::ApplyGlobalVariables(){
    // デバッグ用のグローバル変数の適用など
}

void PlayerAttackBehavior::AddCommand(std::unique_ptr<PlayerAttackCommand> command){
    attackCommands_.push_back(std::move(command));
}
