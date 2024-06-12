#include "Ground.h"

Ground::Ground(Model* model):model_(model){}

Ground::~Ground(){}

void Ground::Initialize(const Vector3& scale){
	worldTransform_.scale_ = scale;
	worldTransform_.rotation_ = {0.0f,0.0f,0.0f};
	worldTransform_.translation_ = {0.0f,0.0f,0.0f};
	worldTransform_.Initialize();
	worldTransform_.UpdateMatrix();
}

void Ground::Update(){

}

void Ground::Draw(const ViewProjection* viewProjection){
	model_->Draw(worldTransform_, *viewProjection);
}