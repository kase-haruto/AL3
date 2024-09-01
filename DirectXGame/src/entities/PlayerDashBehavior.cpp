#include "PlayerDashBehavior.h"
#include "PlayerRootBehavior.h"
#include "Player.h"
#ifdef _DEBUG
#include "GlobalVariables.h"
#endif // _DEBUG

PlayerDashBehavior::PlayerDashBehavior(Player* player)
    : PlayerBaseBehavior(player), dashSpeed(1.0f), dashTime(30), dashParameter_(0){
#ifdef _DEBUG
    const char* groupName = "Player";
    GlobalVariables* globalVariables = GlobalVariables::GetInstance();

    // グループを追加
    globalVariables->AddItem(groupName, "dashSpeed", dashSpeed);
    globalVariables->AddItem(groupName, "dashTime", dashTime);
#endif // _DEBUG
}

PlayerDashBehavior::~PlayerDashBehavior(){}

void PlayerDashBehavior::Initialize(){
    dashParameter_ = 0;
    player_->SetRotationY(player_->GetTargetAngle());

#ifdef _DEBUG
    ApplyGlobalVariables();  // グローバル変数を適用
#endif // _DEBUG
}

void PlayerDashBehavior::Update(){
    player_->MoveInDirection(dashSpeed);

    // 既定の時間経過で通常行動に戻る
    if (++dashParameter_ >= dashTime){
        player_->ChangeState(std::make_unique<PlayerRootBehavior>(player_));
    }
}

void PlayerDashBehavior::ApplyGlobalVariables(){
#ifdef _DEBUG
    GlobalVariables* globalVariables = GlobalVariables::GetInstance();
    const char* groupName = "Player";
    dashSpeed = globalVariables->GetValue<float>(groupName, "dashSpeed");
    dashTime = globalVariables->GetValue<int32_t>(groupName, "dashTime");
#endif // _DEBUG 
}
