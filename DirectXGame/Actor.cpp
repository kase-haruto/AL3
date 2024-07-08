#include "Actor.h"

void Actor::Initialize(const std::vector<Model*>& models){
	models_ = models;
	worldTransform_.Initialize();
}

void Actor::Update(){
	//行列の更新
	worldTransform_.UpdateMatrix();
}

void Actor::Draw(const ViewProjection& viewProjection){
	for (Model* model:models_){
		model->Draw(worldTransform_, viewProjection);
	}
}

///================================
///	アクセッサ
///================================
const WorldTransform& Actor::GetWorldTransform(){ return worldTransform_; }
Vector3 Actor::GetWorldPosition()const{
	Vector3 wPos;
	wPos.x = worldTransform_.matWorld_.m[3][0];
	wPos.y = worldTransform_.matWorld_.m[3][1];
	wPos.z = worldTransform_.matWorld_.m[3][2];
	return wPos;
}
 
Vector3 Actor::GetTranslation()const{ return worldTransform_.translation_; }
Vector3 Actor::GetRotation()const{ return worldTransform_.rotation_; }

void Actor::SetPos(const Vector3& pos){ worldTransform_.translation_ = pos; }
void Actor::SetRotation(const Vector3& rotation){ worldTransform_.rotation_ = rotation; }

uint32_t Actor::GetColor()const{ return color_; }
void Actor::SetColor(const uint32_t& color){ color_ = color; }