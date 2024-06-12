#include "Actor.h"

void Actor::Draw(){
	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);
}

///================================
///	アクセッサ
///================================
Vector3 Actor::GetWorldPosition()const{
	Vector3 wPos;
	wPos.x = worldTransform_.matWorld_.m[3][0];
	wPos.y = worldTransform_.matWorld_.m[3][1];
	wPos.z = worldTransform_.matWorld_.m[3][2];
	return wPos;
}
 
Vector3 Actor::GetTranslation()const{ return worldTransform_.rotation_; }
Vector3 Actor::GetRotation()const{ return worldTransform_.rotation_; }

void Actor::SetTranslatiion(const Vector3& translation){ worldTransform_.translation_ = translation; }
void Actor::SetRotation(const Vector3& rotation){ worldTransform_.rotation_ = rotation; }

uint32_t Actor::GetColor()const{ return color_; }
void Actor::SetColor(const uint32_t& color){ color_ = color; }