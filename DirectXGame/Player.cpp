#include "Player.h"
#include "Input.h"
#include <imgui.h>
Player::Player() {
	model_ = Model::Create();
	textuerHandle_ = TextureManager::Load("./Resources/uvChecker.png");
}

Player::~Player() { delete model_; }

/// <summary>
/// 初期化を行います
/// </summary>
void Player::Init(Vector3 velocity) {
	worldTransform_.Initialize();
	worldTransform_.translation_ = {0.0f,0.0f,0.0f};
	velocity_ = velocity;
}

/// <summary>
/// 移動を行います
/// </summary>
void Player::Move() {
	Input* input = Input::GetInstance();

	if (input->PushKey(DIK_A)) {
		worldTransform_.translation_.x -= velocity_.x;
	} else if (input->PushKey(DIK_D)) {
		worldTransform_.translation_.x += velocity_.x;
	}

	if (input->PushKey(DIK_S)) {
		worldTransform_.translation_.y -= velocity_.y;
	} else if (input->PushKey(DIK_W)) {
		worldTransform_.translation_.y += velocity_.y;
	}
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