#include "FollowCamera.h"
#include"Input.h"
#include"MyFunc.h"
#include"LockOn.h"

FollowCamera::FollowCamera(){}

FollowCamera::~FollowCamera(){}

void FollowCamera::Initialize(){
	viewProjection_.Initialize();
}

void FollowCamera::Update(){
	//追従
	Adulation();
	//ロックオン中
	if (lockOn_->ExistTarget()&&lockOn_){
		ActiveLockOn();
	} else{
		//旋回
		Turning();
	}
	

	//ビュー行列の更新
	viewProjection_.UpdateMatrix();
}

void FollowCamera::Adulation(){
	if (target_){
		//追従座標の補完
		interTarget_ = Lerp(interTarget_, target_->translation_, 0.1f);

		Vector3 offset = CalculateOffset();

		//座標をコピーしてオフセット分ずらす
		viewProjection_.translation_ = interTarget_ + offset;
	}
}

void FollowCamera::Turning(){
	XINPUT_STATE joyState;
	XINPUT_STATE padState;
	DWORD dwResult;

	// ゲームパッドの状態を取得
	dwResult = XInputGetState(0, &padState);

	if (dwResult == ERROR_SUCCESS){
		if (padState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB){
			destinationAngleY_ = target_->rotation_.y;
		}
	}

	if (Input::GetInstance()->GetJoystickState(0, joyState)){
		const float rotateSpeed = 0.1f;
		destinationAngleY_ += ( float ) joyState.Gamepad.sThumbRX / SHRT_MAX * rotateSpeed;
	}

	//最短角度補完
	viewProjection_.rotation_.y = LerpShortAngle(viewProjection_.rotation_.y,destinationAngleY_,0.08f);
}

void FollowCamera::ActiveLockOn(){
	//ロックオン座標
	Vector3 lockOnTargetPos = lockOn_->GetTargetPosition();
	//追従対象者からロックオン対象へのベクトル
	Vector3 sub = lockOnTargetPos - target_->translation_;

	//y軸周りの回転
	viewProjection_.rotation_.y = std::atan2(sub.x, sub.z);
}


void FollowCamera::Reset(){
	//追従対象がいれば
	if (target_){
		//追従座標・角度の初期化
		interTarget_ = target_->translation_;
		viewProjection_.rotation_.y = target_->rotation_.y;
	}
	//追従対象からのオフセット
	Vector3 offset = CalculateOffset();
	viewProjection_.translation_ = interTarget_ + offset;
}

Vector3 FollowCamera::CalculateOffset(){
	//追従対象からカメラも出のオフセット
	Vector3 offset = {0.0f,6.5f,-18.0f};

	//カメラの角度から回転行列を計算
	Matrix4x4 matRotateX = Matrix4x4::MakeRotateXMatrix(viewProjection_.rotation_.x);
	Matrix4x4 matRotateY = Matrix4x4::MakeRotateYMatrix(viewProjection_.rotation_.y);
	Matrix4x4 matRotateZ = Matrix4x4::MakeRotateZMatrix(viewProjection_.rotation_.z);
	Matrix4x4 matRotate = Matrix4x4::Multiply(Matrix4x4::Multiply(matRotateX, matRotateY), matRotateZ);
	offset = Matrix4x4::TransformNormal(offset, matRotate);
	return offset;
}

const ViewProjection& FollowCamera::GetViewProjection(){ return viewProjection_; }

void FollowCamera::SetTarget(const WorldTransform* target){
	target_ = target;
	destination_ = target_->translation_;  // 目標位置を設定
}

void FollowCamera::SetLockOn(const LockOn* lockOn){
	lockOn_ = lockOn;
}

void FollowCamera::SmoothTransition(){
	if (target_){
		// 現在のカメラ位置と目標位置を補間する
		interTarget_ = Lerp(interTarget_, destination_, transitionSpeed_);

		// オフセットを計算して新しいカメラ位置を設定
		Vector3 offset = CalculateOffset();
		viewProjection_.translation_ = interTarget_ + offset;

		// ビュー行列の更新
		viewProjection_.UpdateMatrix();
	}
}