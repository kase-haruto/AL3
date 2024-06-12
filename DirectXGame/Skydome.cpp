#include"Skydome.h"

Skydome::Skydome(Model* model) :model_(model){}

Skydome::~Skydome(){}

void Skydome::Initialize(){
	wTransform_.scale_ = {500.0f, 500.0f, 500.0f};
	wTransform_.rotation_ = {0.0f,0.0f,0.0f};
	wTransform_.translation_ = {0.0f,0.0f,0.0f};
	wTransform_.Initialize();
	wTransform_.UpdateMatrix();
}

void Skydome::Update(){

}

void Skydome::Draw(ViewProjection* viewProjection){
	model_->Draw(wTransform_, *viewProjection);
}