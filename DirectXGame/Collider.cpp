#include "Collider.h"

void Collider::Initialize(){
	worldTransform_.Initialize();
}

void Collider::UpdateTransform(){
	worldTransform_.scale_ = {radius_,radius_,radius_};
	worldTransform_.translation_ = GetCenterPos();
	worldTransform_.UpdateMatrix();
}

void Collider::Draw(Model* model, const ViewProjection& viewProjection){
	model->Draw(worldTransform_, viewProjection);
}

const Vector3& Collider::GetRotation()const{ return worldTransform_.rotation_; }
const Vector3& Collider::GetTranslation()const{ return worldTransform_.translation_; }
const Vector3& Collider::GetScale()const{ return worldTransform_.scale_; }

void Collider::SetRotation(const Vector3& rotation){ worldTransform_.rotation_ = rotation; }
void Collider::SetRotationX(const float rotation){ worldTransform_.rotation_.x = rotation; }
void Collider::SetRotationY(const float rotation){ worldTransform_.rotation_.y = rotation; }
void Collider::SetRotationZ(const float rotation){ worldTransform_.rotation_.z = rotation; }

void Collider::SetTranslation(const Vector3& translation){ worldTransform_.translation_ = translation; }
void Collider::SetTranslationX(const float translation){ worldTransform_.translation_.x = translation; }
void Collider::SetTranslationY(const float translation){ worldTransform_.translation_.y = translation; }
void Collider::SetTranslationZ(const float translation){ worldTransform_.translation_.z = translation; }

void Collider::SetScale(const Vector3& scale){ worldTransform_.scale_ = scale; }