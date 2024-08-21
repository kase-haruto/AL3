#include "Button.h"

void Button::Initialize(const Vector3& pos, const Vector3& scale){
	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;
	worldTransform_.scale_ = scale;
	worldTransform_.UpdateMatrix();
}

void Button::Update(){

}

void Button::Draw(){

}