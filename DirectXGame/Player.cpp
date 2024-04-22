#include "Player.h"
#include <algorithm>
#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG



Player::Player() {
	model_ = Model::Create();
	textuerHandle_ = TextureManager::Load("./Resources/uvChecker.png");
}

Player::~Player() { }

/// <summary>
/// 初期化を行います
/// </summary>
void Player::Init(Vector3 velocity) {
	worldTransform_.Initialize();
	worldTransform_.translation_ = {0.0f,0.0f,0.0f};
	velocity_ = velocity;

	input_ = Input::GetInstance();
}

/// <summary>
/// 移動を行います
/// </summary>
void Player::Move() {
	Vector3 move = {0, 0, 0};

	if (input_->PushKey(DIK_A)) {
		move.x -= velocity_.x;
	} else if (input_->PushKey(DIK_D)) {
		move.x += velocity_.x;
	}

	if (input_->PushKey(DIK_S)) {
		move.y -= velocity_.y;
	} else if (input_->PushKey(DIK_W)) {
		move.y += velocity_.y;
	}

	worldTransform_.translation_ += move;

	//移動限界座標
	const float kMoveLimitX = 32.0f;
	const float kMoveLimitY = 16.0f;

	worldTransform_.translation_.x = std::clamp(worldTransform_.translation_.x, -kMoveLimitX, kMoveLimitX);
	worldTransform_.translation_.y = std::clamp(worldTransform_.translation_.y, -kMoveLimitY, kMoveLimitY);

	worldTransform_.matWorld_ = Matrix4x4::MakeAffineMatrix(worldTransform_.scale_,
															worldTransform_.rotation_,
															worldTransform_.translation_);
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}

/// <summary>
/// 更新処理を行います
/// </summary>
void Player::Update() {

#ifdef _DEBUG
	ImGui::Begin("player");
	ImGui::DragFloat3("translate",&worldTransform_.translation_.x, 0.01f);
	ImGui::End();
#endif // _DEBUG

	Move();
	
}

/// <summary>
/// 描画処理を行いmス
/// </summary>
void Player::Draw(ViewProjection& viewprojection) { 
	model_->Draw(worldTransform_,
				 viewprojection,
				 textuerHandle_);
}