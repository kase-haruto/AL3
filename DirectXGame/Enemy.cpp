#include "Enemy.h"
#include<cassert>

Enemy::Enemy(){

}

Enemy::~Enemy(){

}

void Enemy::Init(Model* model){
	assert(model);
	model_ = model;
	textuerHandle_ = TextureManager::Load("./Resources/cat.png");
	worldTransform_.Initialize();
	worldTransform_.translation_ = {5.0f,0.0f,50.0f};

	ApproachInitialize();
}

void Enemy::ApproachInitialize(){
	//タイマーの初期化
	cooltimer = kShootInterval;
}

void Enemy::ApproachPhase(){
	velocity_ = {0.0f, 0.0f, -0.2f};
	//移動
	worldTransform_.translation_ += velocity_;
	//既定の位置に達したら離脱
	if (worldTransform_.translation_.z <= 0.0f){
		phase_ = Phase::Leave;
	}
	worldTransform_.UpdateMatrix();

	cooltimer--;
	if (cooltimer <= 0){
		Shoot();
		//タイマーの初期化
		cooltimer = kShootInterval;
	}
}

void Enemy::LeavePhase(){
	velocity_ = {0.2f, 0.2f, 0.0f};
	worldTransform_.translation_ += velocity_;
	worldTransform_.UpdateMatrix();
}

void Enemy::Shoot(){

	Vector3 pos = worldTransform_.translation_;
	const float kBulletSpeed = -1.0f;
	Vector3 bulletVel = {0, 0, kBulletSpeed};

	//速度ベクトルを自キャラの向きに合わせて回転
	bulletVel = Vector3::TransformNormal(bulletVel, this->worldTransform_.matWorld_);

	// 弾を生成してユニークポインタにラップ
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Init(model_, pos, bulletVel);

	// 弾を登録
	bullets_.push_back(std::move(newBullet));	
}


void(Enemy::*Enemy::spFuncTable[])() = {
	&Enemy::ApproachPhase,
	&Enemy::LeavePhase
};

void Enemy::Update(){
	if (isMove){

	}
	
	//SwitchPhase();
	(this->*spFuncTable[static_cast<size_t>(phase_)])();
	 
	// 弾の更新
	for (auto& bullet : bullets_){
		bullet->Update();
	}
}

void Enemy::Draw(ViewProjection& viewProjection){
	Actor::Draw(viewProjection);

	for (auto& bullet : bullets_){
		bullet->Draw(viewProjection);
	}

}