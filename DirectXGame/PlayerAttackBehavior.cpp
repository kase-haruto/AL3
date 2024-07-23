#include "PlayerAttackBehavior.h"
#include "PlayerRootBehavior.h"
#include"Player.h"
#include"GlobalVariables.h"
#include "PlayerWeaponSwingDown.h"
#include "PlayerMowDown.h"
#include<Xinput.h>
#include<imgui.h>

PlayerAttackBehavior::PlayerAttackBehavior(Player* player) :currentCommandIndex_(0){
	player_ = player;

	//コンボの追加
	AddCommand(std::make_unique <PlayerMowDown>());
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
	ImGui::Begin("window");
	ImGui::Text("%d", comboReceptionTime_);
	ImGui::End();

	if (currentCommandIndex_ < attackCommands_.size()){
		attackCommands_[currentCommandIndex_]->Execute(player_);

		// 現在の攻撃が終了したら次の攻撃
		if (attackCommands_[currentCommandIndex_]->GetIsFinished()){
			
			if (--comboReceptionTime_ <= 0){
				player_->ChangeState(std::make_unique<PlayerRootBehavior>(player_));
			} else{
				// コンボ受付時間以内
				if (padState.Gamepad.wButtons & XINPUT_GAMEPAD_X){
					// 次の攻撃
					currentCommandIndex_++;
					if (currentCommandIndex_ < attackCommands_.size()){
						attackCommands_[currentCommandIndex_]->Initialize(player_);
						comboReceptionTime_ = comboReceptionTimeMax_;
					}
				}
			}
		}
		

	} else{
		// 全ての攻撃が終了したら通常状態に戻す
		player_->ChangeState(std::make_unique<PlayerRootBehavior>(player_));
	}
}

void PlayerAttackBehavior::ApplyGlobalVariables(){

}

void PlayerAttackBehavior::AddCommand(std::unique_ptr<PlayerAttackCommand> command){
	attackCommands_.push_back(std::move(command));
}
