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

void EnemyBullet::OnCollision(){
	isDead_ = true;
}