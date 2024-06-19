#include "FollowCamera.h"
#include"Input.h"

FollowCamera::FollowCamera(){}

FollowCamera::~FollowCamera(){}

void FollowCamera::Initialize(){
	viewProjection_.Initialize();
}

void FollowCamera::Update(){
	//追従
	Adulation();
	//旋回
	Turning();	

	//ビュー行列の更新
	viewProjection_.UpdateMatrix();
}

void FollowCamera::Adulation(){
	if (target_){
		//追従対象からカメラも出のオフセット
		Vector3 offset = {0.0f,5.0f,-15.0f};

		//カメラの角度から回転行列を計算
		Matrix4x4 matRotateX = Matrix4x4::MakeRotateXMatrix(viewProjection_.rotation_.x);
		Matrix4x4 matRotateY = Matrix4x4::MakeRotateYMatrix(viewProjection_.rotation_.y);
		Matrix4x4 matRotateZ = Matrix4x4::MakeRotateZMatrix(viewProjection_.rotation_.z);
		Matrix4x4 matRotate = Matrix4x4::Multiply(Matrix4x4::Multiply(matRotateX, matRotateY), matRotateZ);
		offset = Matrix4x4::TransformNormal(offset, matRotate);

		//座標をコピーしてオフセット分ずらす
		viewProjection_.translation_ = target_->translation_ + offset;
	}
}

void FollowCamera::Turning(){
	XINPUT_STATE joyState;
	
	if (Input::GetInstance()->GetJoystickState(0, joyState)){
		const float rotateSpeed = 0.1f;
		viewProjection_.rotation_.y += ( float ) joyState.Gamepad.sThumbRX / SHRT_MAX * rotateSpeed;
	}
}

const ViewProjection& FollowCamera::GetViewProjection(){ return viewProjection_; }