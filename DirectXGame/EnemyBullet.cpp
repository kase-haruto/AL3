#include "EnemyBullet.h"

EnemyBullet::EnemyBullet(){

}

EnemyBullet::~EnemyBullet(){

}

void EnemyBullet::Init(Model* model, const Vector3& position, const Vector3& vel){
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	textuerHandle_ = TextureManager::Load("./Resources/sample.png");
	velocity_ = vel;
	radius_ = 1.0f;

	const uint32_t kCollisionAttribute = 0b1 << 1;
	SetCollisionAttribute(kCollisionAttribute);//敵陣営
	SetCollisionMask(~kCollisionAttribute);//敵陣営以外
}

void EnemyBullet::Update(){
	worldTransform_.translation_ += velocity_;
	worldTransform_.UpdateMatrix();

	//時間経過でデス
	if (--deathTimer_ <= 0){
		isDead_ = true;
	}
}

void EnemyBullet::Draw(const ViewProjection& viewProjection){
	if (!isDead_){
		Bullet::Draw(viewProjection);
	}
}	

Vector3 EnemyBullet::GetWorldPosition()const{
	Vector3 wPos;
	wPos = worldTransform_.translation_;
	return wPos;
}

void EnemyBullet::OnCollision(){
	isDead_ = true;
}