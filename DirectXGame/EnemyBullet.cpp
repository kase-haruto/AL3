#include "EnemyBullet.h"

EnemyBullet::EnemyBullet(){

}

EnemyBullet::~EnemyBullet(){

}

void EnemyBullet::Init(Model* model, const Vector3& position, const Vector3& vel){
	model_ = model;
	textuerHandle_ = TextureManager::Load("./Resources/sample.png");
	velocity_ = vel;
	worldTransform_.Initialize();
	//弾を長細くする
	worldTransform_.scale_.x = 0.5f;
	worldTransform_.scale_.y = 0.5f;
	worldTransform_.scale_.z = 3.0f;
	//y軸周りの角度
	worldTransform_.rotation_.y = std::atan2(vel.x,vel.z);
	worldTransform_.rotation_.x = std::atan2(vel.y,vel.z);
	worldTransform_.translation_ = position;
	
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