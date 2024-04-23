#include "PlayerBullet.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity){
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	textuerHandle_ = TextureManager::Load("./Resources/white1x1.png");
	velocity_ = velocity;
}

void PlayerBullet::Update(){
	worldTransform_.translation_ += velocity_;
	worldTransform_.UpdateMatrix();
}

void PlayerBullet::Draw(const ViewProjection& viewProcetion){
	Bullet::Draw(viewProcetion);
}