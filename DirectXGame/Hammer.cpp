#include "Hammer.h"
#include<cassert>
void Hammer::Initialize(Model* model){
	WeaponBase::Initialize(model);
}

void Hammer::Draw(const ViewProjection& viewProjection){
	WeaponBase::Draw(viewProjection);
}

void Hammer::OnCollision(){}

Vector3 Hammer::GetCenterPos() const{
	const Vector3 offset = {0.0f,1.5f,0.0f};
	Vector3 worldPos = Matrix4x4::Transform(offset, worldTransform_.matWorld_);
	return worldPos;
}