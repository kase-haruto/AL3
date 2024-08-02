#include "WeaponBase.h"
#include<cassert>
#include"CollisionTypeIdDef.h"

void WeaponBase::Initialize(Model* model){
	worldTransform_.Initialize();
	model_ = model;
	Collider::SetTypeID(static_cast< uint32_t >(CollisionTypeIdDef::kPlayerWeapon));
}

void WeaponBase::Draw(const ViewProjection& viewProjection){
	assert(model_);
	model_->Draw(worldTransform_, viewProjection);

}



void WeaponBase::SetModel(Model* model){ model_ = model; }