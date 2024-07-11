#include "Player.h"
#include"TextureManager.h"
#include"Input.h"
#include"GlobalVariables.h"
#ifdef _DEBUG
#include<imgui.h>
#endif // DEBUG

#include<cassert>
#include<numbers>

///=======================================================================================================
///		グローバル変数の調整項目の適用
///=======================================================================================================
void Player::ApplyGlobalVariables(){
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	partsTransform_[static_cast<int>(Parts::head)]->translation_ = globalVariables->GetValue<Vector3>(groupName, "Head Translation");
	partsTransform_[static_cast<int>(Parts::L_arm)]->translation_ = globalVariables->GetValue<Vector3>(groupName, "ArmL Translation");
	partsTransform_[static_cast<int>(Parts::R_arm)]->translation_ = globalVariables->GetValue<Vector3>(groupName, "ArmR Translation");
	cycle_ = globalVariables->GetValue<int32_t>(groupName, "floatingCycle");
	floatingAmplitude = globalVariables->GetValue<float>(groupName, "floatingAmplitude");
}

Player::Player(){
	//パーツの要素数
	partsTransform_.resize(static_cast< int >(Parts::partsCount));
	//パーツごとのtransform
	partsTransform_[static_cast< int >(Parts::body)] = std::make_unique<WorldTransform>();
	partsTransform_[static_cast< int >(Parts::head)] = std::make_unique<WorldTransform>();
	partsTransform_[static_cast< int >(Parts::L_arm)] = std::make_unique<WorldTransform>();
	partsTransform_[static_cast< int >(Parts::R_arm)] = std::make_unique<WorldTransform>();
	partsTransform_[static_cast< int >(Parts::weapon)] = std::make_unique<WorldTransform>();
}

Player::~Player(){}

///=======================================================================================================
///		初期化/更新/描画
///=======================================================================================================
void Player::Initialize(const std::vector<Model*>& models){
	//モデルとトランスフォームの初期化
	Actor::Initialize(models);
	//各パーツのトランスフォームの初期化
	PartsTransformInit();
	//浮遊ギミックの初期化
	InitializeFloatingAction();

	const char* groupName = "Player";
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	globalVariables->CreateGroup(groupName);

	//グループを追加
	globalVariables->AddItem(groupName, "Head Translation", partsTransform_[static_cast< int >(Parts::head)]->translation_);
	globalVariables->AddItem(groupName, "ArmL Translation", partsTransform_[static_cast< int >(Parts::L_arm)]->translation_);
	globalVariables->AddItem(groupName, "ArmR Translation", partsTransform_[static_cast< int >(Parts::R_arm)]->translation_);
	globalVariables->AddItem(groupName, "floatingCycle", cycle_);
	globalVariables->AddItem(groupName, "floatingAmplitude", floatingAmplitude);

	ApplyGlobalVariables();
}

void Player::PartsTransformInit(){
	for (int i = 0; i < (int)Parts::partsCount; i++){
		partsTransform_[i]->Initialize();
	}
	//====================================================================================
	//各パーツごとのポジションを設定
	Vector3 headPos {0.0f,2.75f,0.0f};
	Vector3 L_armPos {-1.0f,2.0f,0.0f};
	Vector3 R_armPos {1.0f,2.0f,0.0f};
	partsTransform_[static_cast< int >(Parts::head)]->translation_ = headPos;
	partsTransform_[static_cast< int >(Parts::L_arm)]->translation_ = L_armPos;
	partsTransform_[static_cast< int >(Parts::R_arm)]->translation_ = R_armPos;
	//=====================================================================================

	//====================================================================================
	//親子関係を結ぶ
	partsTransform_[static_cast< int >(Parts::body)]->parent_ = &worldTransform_;
	auto body = partsTransform_[static_cast< int >(Parts::body)].get();
	partsTransform_[static_cast< int >(Parts::head)]->parent_ = body;
	partsTransform_[static_cast< int >(Parts::L_arm)]->parent_ = body;
	partsTransform_[static_cast< int >(Parts::R_arm)]->parent_ = body;
	partsTransform_[static_cast< int >(Parts::weapon)]->parent_ = body;
	//====================================================================================
}

void Player::Update(){

#ifdef _DEBUG
	ImGui::Begin("player");
	ImGui::DragFloat3("translation", &worldTransform_.translation_.x, 0.01f);
	const char* partNames[] = {"head", "body", "L_arm", "R_arm","weapon"};
	Parts partIndices[] = {Parts::head, Parts::body, Parts::L_arm, Parts::R_arm,Parts::weapon};

	for (int i = 0; i < 5; ++i){
		if (ImGui::TreeNode(partNames[i])){
			ImGui::DragFloat3((std::string(partNames[i]) + ".translation").c_str(), &partsTransform_[static_cast< int >(partIndices[i])]->translation_.x, 0.01f);
			ImGui::DragFloat3((std::string(partNames[i]) + ".rotation").c_str(), &partsTransform_[static_cast< int >(partIndices[i])]->rotation_.x, 0.01f);
			ImGui::TreePop();
		}
	}
	ImGui::End();
#endif // _DEBUG

	// 通常の更新処理
	TrasitionaBehavior();
	BehaviorUpdate();

	//最短角度補完
	worldTransform_.rotation_.y = LerpShortAngle(worldTransform_.rotation_.y, targetAngle, 0.1f);

	//transformの初期化
	Actor::Update();
	for (const auto& transform : partsTransform_){
		transform->UpdateMatrix();
	}
}

void Player::Draw(const ViewProjection& viewProjection){
	//各パーツごとの描画
	models_[static_cast< int >(Parts::head)]->Draw(*partsTransform_[static_cast< int >(Parts::head)], viewProjection);
	models_[static_cast< int >(Parts::body)]->Draw(*partsTransform_[static_cast< int >(Parts::body)], viewProjection);
	models_[static_cast< int >(Parts::L_arm)]->Draw(*partsTransform_[static_cast< int >(Parts::L_arm)], viewProjection);
	models_[static_cast< int >(Parts::R_arm)]->Draw(*partsTransform_[static_cast< int >(Parts::R_arm)], viewProjection);

	if (isAttack_){
		models_[static_cast< int >(Parts::weapon)]->Draw(*partsTransform_[static_cast< int >(Parts::weapon)], viewProjection);
	}
}


///=======================================================================================================
///		通常行動時の処理
///=======================================================================================================
void Player::Move(){
	XINPUT_STATE joyState;
	// 移動量
	if (Input::GetInstance()->GetJoystickState(0, joyState)){
		const float threshold = 0.7f;
		bool isMoving = false;
		// 速さ
		const float speed = 0.3f;
		// 移動量
		Vector3 move = {( float ) joyState.Gamepad.sThumbLX / SHRT_MAX, 0.0f, ( float ) joyState.Gamepad.sThumbLY / SHRT_MAX};
		if (Length(move) > threshold){
			isMoving = true;
		}

		if (isMoving){
			// 移動量に速さを反映
			direction_ = Normalize(move);

			//向いている方向に移動
			MoveInDirection(speed);

		}
	}
}

void Player::MoveInDirection(float speed){
	Vector3 move = direction_ * speed;

	Vector3 rotate = viewPorjection_->rotation_;

	// カメラの角度から回転行列を計算
	Matrix4x4 matRotateY = Matrix4x4::MakeRotateYMatrix(rotate.y);
	Matrix4x4 matRotateZ = Matrix4x4::MakeRotateZMatrix(rotate.z);
	Matrix4x4 matRotate = Matrix4x4::Multiply(matRotateY, matRotateZ);
	move = Matrix4x4::Transform(move, matRotate);

	// 実際の移動
	float horizontalDistance = sqrtf(move.x * move.x + move.z * move.z);
	worldTransform_.rotation_.x = std::atan2(-move.y, horizontalDistance);

	worldTransform_.translation_ += move;

	// 振り向きの目標角度を設定
	targetAngle = std::atan2(move.x, move.z);
}

void Player::InitializeFloatingAction(){
	floatingParameter_ = 1.0f;
	floatingAmplitude = 0.45f;
}

void Player::UpdateFloatingAction(){
	//浮遊移動のサイクル
	//１フレームでのパラメータ加算値
	const float step = 2.0f * ( float ) std::numbers::pi / cycle_;
	//パラメータ1ステップ分加算
	floatingParameter_ += step;
	//2piを超えたら0に戻す
	floatingParameter_ = std::fmod(floatingParameter_, 2.0f * ( float ) std::numbers::pi);
	
	//浮遊を座標に反映
	partsTransform_[static_cast< int >(Parts::body)]->translation_.y = std::sin(floatingParameter_) * floatingAmplitude;
}

void Player::RootInitialize(){
	isAttack_ = false;
	partsTransform_[static_cast< int >(Parts::L_arm)]->rotation_.x = 0.0f;
	partsTransform_[static_cast< int >(Parts::R_arm)]->rotation_.x = 0.0f;
}

void Player::BehaviorRootUpdate(){
	XINPUT_STATE padState;
	DWORD dwResult;

	// ゲームパッドの状態を取得
	dwResult = XInputGetState(0, &padState);

	if (dwResult == ERROR_SUCCESS){
		if (padState.Gamepad.wButtons & XINPUT_GAMEPAD_X){
			behaviorRequest_ = Behavior::attack;
		}

		if (padState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER){
			behaviorRequest_ = Behavior::dash;
		}
	}
	

	//移動処理
	Move();
	//浮遊ギミックの処理
	UpdateFloatingAction();
}


///=======================================================================================================
///		攻撃時の処理
///=======================================================================================================
void Player::AttackInitialize(){
	isAttack_ = true;
	float shakeUpAngle = -3.1f;
	partsTransform_[static_cast< int >(Parts::weapon)]->rotation_.x = 0.0f;
	partsTransform_[static_cast< int >(Parts::L_arm)]->rotation_.x = shakeUpAngle;
	partsTransform_[static_cast< int >(Parts::R_arm)]->rotation_.x = shakeUpAngle;
}

void Player::BehaviorAttackUpdate(){
	float targetArmAngle = 1.3f;
	auto& weaponAngle = partsTransform_[static_cast< int >(Parts::weapon)]->rotation_;
	auto& L_armAngle = partsTransform_[static_cast< int >(Parts::L_arm)]->rotation_;
	auto& R_armAngle = partsTransform_[static_cast< int >(Parts::R_arm)]->rotation_;

	weaponAngle.x = Lerp(weaponAngle.x, targetArmAngle, 0.2f);
	L_armAngle.x = Lerp(L_armAngle.x, -targetArmAngle, 0.2f);
	R_armAngle.x = Lerp(R_armAngle.x, -targetArmAngle, 0.2f);


	// 目標角度に達したら攻撃を初期化
	if (std::abs(targetArmAngle - weaponAngle.x) <= 0.001f){
		behaviorRequest_ = Behavior::root;
	}
}


///=======================================================================================================
///		ダッシュの処理
///=======================================================================================================
void Player::BehaviorDashInitialize(){
	workDash_.dashParameter_ = 0;
	worldTransform_.rotation_.y = targetAngle;

}

void Player::BehaviorDashUpdate(){
	// ダッシュの速さ
	const float dashSpeed = 0.6f;
	MoveInDirection(dashSpeed);


	//ダッシュ時間
	const uint32_t dashTime = 30;
	//既定の時間経過で通常行動に戻る
	if (++workDash_.dashParameter_ >= dashTime){
		//ダッシュが終了したら通常行動に戻る
		behaviorRequest_ = Behavior::root;
	}
}


///=======================================================================================================
///		プレイヤーの状態の処理
///=======================================================================================================
void Player::TrasitionaBehavior(){
	if (behaviorRequest_){
		//ふるまいの変更
		behavior_ = behaviorRequest_.value();
		//各振る舞いごとの初期化
		switch (behavior_){
			case Behavior::root:
			default:
				RootInitialize();
				break;

			case Behavior::attack:
				AttackInitialize();
				break;

			case Behavior::dash:
				BehaviorDashInitialize();
		}
		//ふるまいリクエストをリセット
		behaviorRequest_ = std::nullopt;
	}
}

void Player::BehaviorUpdate(){
	switch (behavior_){
		//通常
		case Behavior::root:
		default:
			BehaviorRootUpdate();
			break;
			//攻撃
		case Behavior::attack:
			BehaviorAttackUpdate();
			break;
			//ダッシュ
		case Behavior::dash:
			BehaviorDashUpdate();
	}
}



void Player::SetViewProjection(const ViewProjection* viewProjection){ viewPorjection_ = viewProjection; }

