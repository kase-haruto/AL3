#include "PlayerRootBehavior.h"
#include"Player.h"
#include<numbers>	
#include"Input.h"
#include"GlobalVariables.h"

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

	//グループを追加
	globalVariables->AddItem(groupName, "moveSpeed", speed_);
	globalVariables->AddItem(groupName, "floatingCycle", cycle_);
	globalVariables->AddItem(groupName, "floatingAmplitude", floatingAmplitude_);
}

PlayerRootBehavior::~PlayerRootBehavior(){}


void PlayerRootBehavior::Initialize(){
	player_->SetIsAttack(false);
	Vector3 L_Arm_Rotate  = {0.0f,player_->GetPartsTransform(int(Parts::L_arm))->rotation_.y,player_->GetPartsTransform(int(Parts::L_arm))->rotation_.z};
	Vector3 R_Arm_Rotate  = {0.0f,player_->GetPartsTransform(int(Parts::R_arm))->rotation_.y,player_->GetPartsTransform(int(Parts::R_arm))->rotation_.z};
	player_->Set_L_ArmRotation(L_Arm_Rotate);
	player_->Set_R_ArmRotation(R_Arm_Rotate);
}

void PlayerRootBehavior::Update(){
	XINPUT_STATE padState;
	DWORD dwResult;

	//調整項目の適用
	ApplyGlobalVariables();	

	// ゲームパッドの状態を取得
	dwResult = XInputGetState(0, &padState);

	if (dwResult == ERROR_SUCCESS){
		if (padState.Gamepad.wButtons & XINPUT_GAMEPAD_X){
			player_->SetBehavior(Behavior::attack);
		}

		if (padState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER){
			player_->SetBehavior(Behavior::dash);
		}

		//ジャンプボタンを押したら
		if (padState.Gamepad.wButtons & XINPUT_GAMEPAD_A){
			player_->SetBehavior(Behavior::jump);
		}
	}

	//移動処理
	Move();
	//浮遊ギミックの処理
	UpdateFloatingAction();
}

void PlayerRootBehavior::Move(){
	XINPUT_STATE joyState;
	// 移動量
	if (Input::GetInstance()->GetJoystickState(0, joyState)){
		const float threshold = 0.7f;
		bool isMoving = false;
		
		// 移動量
		Vector3 vel = {( float ) joyState.Gamepad.sThumbLX / SHRT_MAX, 0.0f, ( float ) joyState.Gamepad.sThumbLY / SHRT_MAX};
		player_->SetVelocity(vel);
		if (Length(vel) > threshold){
			isMoving = true;
		}

		if (isMoving){
			// 移動量に速さを反映
			player_->SetDirection(Normalize(player_->GetVelocity()));

			//向いている方向に移動
			player_->MoveInDirection(speed_);

		}
	}
}

void PlayerRootBehavior::InitializeFloatingAction(){
	floatingParameter_ = 1.0f;
	floatingAmplitude_ = 0.45f;
}

void PlayerRootBehavior::UpdateFloatingAction(){
	//浮遊移動のサイクル
	//１フレームでのパラメータ加算値
	const float step = 2.0f * ( float ) std::numbers::pi / cycle_;
	//パラメータ1ステップ分加算
	floatingParameter_ += step;
	//2piを超えたら0に戻す
	floatingParameter_ = std::fmod(floatingParameter_, 2.0f * ( float ) std::numbers::pi);

	//浮遊を座標に反映
	player_->SetBodyTranslationY(std::sin(floatingParameter_) * floatingAmplitude_);
}