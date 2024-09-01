#include "Button.h"
#include"TextureManager.h"
#ifdef _DEBUG
#include<imgui.h>
#endif // _DEBUG


void Button::Initialize(const Vector3& pos, const Vector2& scale, const std::string& filePath){
	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;
	worldTransform_.UpdateMatrix();
	textureHandle_ = TextureManager::Load(filePath.c_str());

	sprite_.reset(Sprite::Create(textureHandle_, {pos.x,pos.y}, {1, 1, 1, 1},{0.5f,0.5f}));
	sprite_->SetSize(scale);
}

void Button::Update(){

	if (isSelected_){
		sprite_->SetSize({400.0f,150.0f});
	} else{
		sprite_->SetSize({300.0f,100.0f});
	}
}

void Button::Draw(){
	sprite_->Draw();
}

void Button::SetIsSelected(bool isSelect){
	isSelected_ = isSelect;
}

Vector2 Button::GetPos() const{
	return sprite_->GetPosition();
}

void Button::SetPos(const Vector2& pos){
	sprite_->SetPosition(pos);
}
