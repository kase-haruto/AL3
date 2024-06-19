#include "Player.h"
#include"TextureManager.h"
#include"Input.h"
#include<cassert>

Player::Player(){}

Player::~Player(){}

void Player::Initialize(Model* model){
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	textureHandle_ = TextureManager::Load("uvChecker.png");

}

void Player::Update(){
	Move();
	//最短角度補完
	worldTransform_.rotation_.y = LerpShortAngle(worldTransform_.rotation_.y, targetAngle, 0.1f);
	//行列の更新
	worldTransform_.UpdateMatrix();
}

void Player::Draw(ViewProjection& viewProjection){
	Actor::Draw(viewProjection);
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

void Player::SetViewProjection(const ViewProjection* viewProjection){ viewPorjection_ = viewProjection; }