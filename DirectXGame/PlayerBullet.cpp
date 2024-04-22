#include "PlayerBullet.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position){
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	textuerHandle_ = TextureManager::Load("./Resources/white1x1.png");
}

void PlayerBullet::Update(){
	worldTransform_.UpdateMatrix();
}

void PlayerBullet::Draw(const ViewProjection& viewProcetion){
	Bullet::Draw(viewProcetion);
}