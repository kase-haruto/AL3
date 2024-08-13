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



void WeaponBase::ClearContactRecord(){
	contactRecord_.Clear();
}


const Vector3& WeaponBase::GetRotation()const{ return worldTransform_.rotation_; }
const Vector3& WeaponBase::GetTranslation()const{ return worldTransform_.translation_; }


void WeaponBase::SetModel(Model* model){ model_ = model; }

void WeaponBase::SetParent(WorldTransform worldTransform){
	worldTransform_.parent_ = &worldTransform;
}