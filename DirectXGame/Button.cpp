#include "Button.h"
#include"TextureManager.h"
#include<imgui.h>

void Button::Initialize(const Vector3& pos, const Vector3& scale, const std::string& filePath){
	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;
	worldTransform_.scale_ = scale;
	worldTransform_.UpdateMatrix();
	textureHandle_ = TextureManager::Load(filePath.c_str());

	sprite_.reset(Sprite::Create(textureHandle_, {pos.x,pos.y}, {1, 1, 1, 1},{0.5f,0.5f}));
	sprite_->SetSize({50.0f,50.0f});
}

void Button::Update(){

	if (isSelected_){
		sprite_->SetSize({100.0f,100.0f});
	} else{
		sprite_->SetSize({50.0f,50.0f});
	}
}

void Button::Draw(){
	sprite_->Draw();
}

void Button::SetIsSelected(bool isSelect){
	isSelected_ = isSelect;
}