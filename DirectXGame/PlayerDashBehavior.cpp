#include "PlayerDashBehavior.h"
#include"PlayerRootBehavior.h"
#include"Player.h"
#include"GlobalVariables.h"

PlayerDashBehavior::PlayerDashBehavior(Player* player){
	player_ = player;

	const char* groupName = "Player";
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();

	//グループを追加
	globalVariables->AddItem(groupName, "dashSpeed", dashSpeed);
	globalVariables->AddItem(groupName, "dashTime", dashTime);
}

PlayerDashBehavior::~PlayerDashBehavior(){}


void PlayerDashBehavior::Initialize(){
	dashParameter_ = 0;
	player_->SetRotationY(player_->GetTargetAngle());
}

void PlayerDashBehavior::Update(){

	player_->MoveInDirection(dashSpeed);
	
	//既定の時間経過で通常行動に戻る
	if (++dashParameter_ >= dashTime){
		//ダッシュが終了したら通常行動に戻る
		player_->ChangeState(std::make_unique<PlayerRootBehavior>(player_));
	}
}

void PlayerDashBehavior::ApplyGlobalVariables(){
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	dashSpeed = globalVariables->GetValue<float>(groupName, "dashSpeed");
	dashTime = globalVariables->GetValue<int32_t>(groupName, "dashTime");
}