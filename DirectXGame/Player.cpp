#include "Player.h"
#include"TextureManager.h"
#include"Input.h"

#ifdef _DEBUG
#include<imgui.h>
#endif // DEBUG

#include<cassert>
#include<numbers>

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

void Player::Initialize(const std::vector<Model*>& models){
	//モデルとワールドトランスフォームの初期化
	Actor::Initialize(models);
	for (int i = 0; i < models.size(); i++){
		partsTransform_[i]->Initialize();
	}

	//親子関係を結ぶ
	partsTransform_[static_cast< int >(Parts::body)]->parent_ = &worldTransform_;
	partsTransform_[static_cast< int >(Parts::head)]->parent_ = partsTransform_[static_cast< int >(Parts::body)].get();
	partsTransform_[static_cast< int >(Parts::L_arm)]->parent_ = partsTransform_[static_cast< int >(Parts::body)].get();
	partsTransform_[static_cast< int >(Parts::R_arm)]->parent_ = partsTransform_[static_cast< int >(Parts::body)].get();
	partsTransform_[static_cast< int >(Parts::weapon)]->parent_ = partsTransform_[static_cast<int>(Parts::body)].get();
	//浮遊ギミックの初期化
	InitializeFloatingAction();
}

void Player::Update(){

#ifdef _DEBUG
	ImGui::Begin("player");
	ImGui::DragFloat3("translation", &worldTransform_.translation_.x, 0.01f);
	const char* partNames[] = {"head", "body", "L_arm", "R_arm","weapon"};
	Parts partIndices[] = {Parts::head, Parts::body, Parts::L_arm, Parts::R_arm,Parts::weapon};

	for (int i = 0; i < 5; ++i){
		if (ImGui::TreeNode(partNames[i])){
			ImGui::DragFloat3((std::string(partNames[i]) + ".rotation").c_str(), &partsTransform_[static_cast< int >(partIndices[i])]->rotation_.x, 0.01f);
			ImGui::TreePop();
		}
	}
	ImGui::End();
#endif // _DEBUG

	BehaviorRootUpdate();
	if (isAttack_){
		BehaviorAttackUpdate();
	}

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
	models_[static_cast< int >(Parts::weapon)]->Draw(*partsTransform_[static_cast< int >(Parts::weapon)], viewProjection);
}

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
			move = Normalize(move) * speed;

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
	}
}

void Player::InitializeFloatingAction(){
	floatingParameter_ = 0.0f;
}

void Player::UpdateFloatingAction(){
	//浮遊移動のサイクル
	const uint16_t cycle = 120;
	//１フレームでのパラメータ加算値
	const float step = 2.0f * ( float ) std::numbers::pi / cycle;
	//パラメータ1ステップ分加算
	floatingParameter_ += step;
	//2piを超えたら0に戻す
	floatingParameter_ = std::fmod(floatingParameter_, 2.0f * ( float ) std::numbers::pi);
	//浮遊の振幅
	const float floatingAmplitude = 0.4f;
	//浮遊を座標に反映
	partsTransform_[static_cast< int >(Parts::body)]->translation_.y = std::sin(floatingParameter_) * floatingAmplitude;
}

void Player::BehaviorRootUpdate(){
	//攻撃
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)){
		isAttack_ = true;
	}

	//移動処理
	Move();
	//浮遊ギミックの処理
	UpdateFloatingAction();
}

void Player::BehaviorAttackUpdate(){
	partsTransform_[static_cast< int >(Parts::L_arm)]->rotation_.x = 180;
	partsTransform_[static_cast< int >(Parts::R_arm)]->rotation_.x = 180;
	isAttack_ = false;
}

void Player::SetViewProjection(const ViewProjection* viewProjection){ viewPorjection_ = viewProjection; }

