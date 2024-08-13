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