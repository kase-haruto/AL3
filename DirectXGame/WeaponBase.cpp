#include "WeaponBase.h"
#include<cassert>

void WeaponBase::Initialize(Model* model){
	worldTransform_.Initialize();
	model_ = model;

	Collider::Initialize();
}

void WeaponBase::Draw(const ViewProjection& viewProjection){
	assert(model_);
	model_->Draw(worldTransform_, viewProjection);

}

const Vector3& WeaponBase::GetRotation()const{ return worldTransform_.rotation_; }
const Vector3& WeaponBase::GetTranslation()const{ return worldTransform_.translation_; }

void WeaponBase::SetRotation(const Vector3& rotation){ worldTransform_.rotation_ = rotation; }
void WeaponBase::SetRotationX(const float rotation){ worldTransform_.rotation_.x = rotation; }
void WeaponBase::SetRotationY(const float rotation){ worldTransform_.rotation_.y = rotation; }
void WeaponBase::SetRotationZ(const float rotation){ worldTransform_.rotation_.z = rotation; }

void WeaponBase::SetTranslation(const Vector3& translation){ worldTransform_.translation_ = translation; }
void WeaponBase::SetTranslationX(const float translation){ worldTransform_.translation_.x = translation; }
void WeaponBase::SetTranslationY(const float translation){ worldTransform_.translation_.y = translation; }
void WeaponBase::SetTranslationZ(const float translation){ worldTransform_.translation_.z = translation; }

void WeaponBase::SetModel(Model* model){ model_ = model; }

void WeaponBase::SetParent(WorldTransform worldTransform){
	worldTransform_.parent_ = &worldTransform;
}