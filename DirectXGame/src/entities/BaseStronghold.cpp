#include "BaseStronghold.h"

void BaseStronghold::Initialize(Model* model){
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.UpdateMatrix();

	domeTransform_.Initialize();
	domeTransform_.UpdateMatrix();

	rangeRadius_ = 1.0f;
	life_ = 5;
}

void BaseStronghold::Draw(const ViewProjection& viewProjection){
	if (!isTaked_){
		model_->Draw(worldTransform_, viewProjection);
	}
}

void BaseStronghold::DrawDome(const ViewProjection& viewProjection){
	if (rangeModel_){
		rangeModel_->Draw(domeTransform_, viewProjection);
	}
}

void BaseStronghold::ReduceLife(){
	const uint16_t oneSecond = 60;

	// フレームカウンターをインクリメント
	holdTimer_++;

	// 60フレームが経過したかどうかをチェック
	if (holdTimer_ >= oneSecond){

		// life_ を1減らす
		if (life_ > 0){
			life_--;
		}

		// フレームカウンターをリセット
		holdTimer_ = 0;
	}

	if (life_<= 0.0f){
		isTaked_ = true;
	}
}

void BaseStronghold::UpdateMatrix(){
	worldTransform_.UpdateMatrix();
}


///========================================================================================
///								アクセッサ
///========================================================================================


uint32_t BaseStronghold::GetLife()const{ return life_; }

Vector3 BaseStronghold::GetCenterPos()const{
	const Vector3 offset = {0.0f,1.5f,0.0f};
	Vector3 worldPos = Matrix4x4::Transform(offset, worldTransform_.matWorld_);
	return worldPos;
}

float BaseStronghold::GetRadius() const{
	return rangeRadius_;
}

Vector3 BaseStronghold::GetWorldPosition()const{
	Vector3 wPos;
	wPos.x = worldTransform_.matWorld_.m[3][0];
	wPos.y = worldTransform_.matWorld_.m[3][1];
	wPos.z = worldTransform_.matWorld_.m[3][2];
	return wPos;
}

bool BaseStronghold::GetIsTaked() const{
	return isTaked_;
}

const WorldTransform& BaseStronghold::GetWorldTransform()const{ return worldTransform_; }


void BaseStronghold::SetLife(const uint32_t life){ life_ = life; }

void BaseStronghold::SetRotate(const Vector3& rotate){ worldTransform_.rotation_ = rotate; }

void BaseStronghold::SetTranslation(const Vector3& translate){ worldTransform_.translation_ = translate; }

void BaseStronghold::SetScale(const Vector3& scale){ worldTransform_.scale_ = scale; }

void BaseStronghold::SetIsTaked(bool isTaked){
	isTaked_ = isTaked;
}

void BaseStronghold::SetRangeModel(Model* model){
	rangeModel_ = model;
	rangeModel_->SetAlpha(0.4f);
}
