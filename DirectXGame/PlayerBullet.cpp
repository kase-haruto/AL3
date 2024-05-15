#include "PlayerBullet.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity){
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	textuerHandle_ = TextureManager::Load("./Resources/white1x1.png");
	velocity_ = velocity;
	radius_ = 1.0f;
	
	const uint32_t kCollisionAttribute = 0b1;
	SetCollisionAttribute(kCollisionAttribute);
	SetCollisionMask(~kCollisionAttribute);
}

void PlayerBullet::Update(){
	worldTransform_.translation_ += velocity_;
	worldTransform_.UpdateMatrix();

	//時間経過でデス
	if (--deathTimer_ <= 0){
		isDead_ = true;
	}
}

void PlayerBullet::Draw(const ViewProjection& viewProcetion){
	if (!isDead_){
		Bullet::Draw(viewProcetion);
	}
}

void PlayerBullet::OnCollision(){
	isDead_ = true;
}

Vector3 PlayerBullet::GetWorldPosition()const{
	Vector3 wPos;
	wPos = worldTransform_.translation_;
	return wPos;
}