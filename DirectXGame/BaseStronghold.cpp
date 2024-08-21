#include "BaseStronghold.h"

void BaseStronghold::Initialize(Model* model){
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.UpdateMatrix();
}

void BaseStronghold::Draw(const ViewProjection& viewProjection){
	model_->Draw(worldTransform_,viewProjection);
	if (rangeModel_){
		rangeModel_->Draw(worldTransform_, viewProjection);
	}
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

const WorldTransform& BaseStronghold::GetWorldTransform()const{ return worldTransform_; }


void BaseStronghold::SetLife(const uint32_t life){ life_ = life; }

void BaseStronghold::SetRotate(const Vector3& rotate){ worldTransform_.rotation_ = rotate; }

void BaseStronghold::SetTranslation(const Vector3& translate){ worldTransform_.translation_ = translate; }

void BaseStronghold::SetScale(const Vector3& scale){ worldTransform_.scale_ = scale; }