#include "WeaponBase.h"
#include<cassert>

void WeaponBase::Initialize(){
	worldTransform_.Initialize();
}

void WeaponBase::Draw(const ViewProjection& viewProjection){
	assert(model_);
	model_->Draw(worldTransform_, viewProjection);

}

const Vector3& WeaponBase::GetRotation()const{ return worldTransform_.rotation_; }
const Vector3& WeaponBase::GetTranslation()const{ return worldTransform_.translation_; }

void WeaponBase::SetRotation(const Vector3& rotation){ worldTransform_.rotation_ = rotation; }
void WeaponBase::SetTranslation(const Vector3& translation){ worldTransform_.translation_ = translation; }
void WeaponBase::SetModel(Model* model){ model_ = model; }