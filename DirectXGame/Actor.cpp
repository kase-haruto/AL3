#include "Actor.h"

void Actor::Initialize(const std::vector<Model*>& models){
	models_ = models;
	worldTransform_.Initialize();

	Collider::Initialize();
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

Vector3 Actor::GetCenterPos()const{
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
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

void Actor::SetPos(const Vector3& pos){ worldTransform_.translation_ = pos; }


uint32_t Actor::GetColor()const{ return color_; }
void Actor::SetColor(const uint32_t& color){ color_ = color; }