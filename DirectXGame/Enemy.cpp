#include "Enemy.h"
#include<cassert>
#include"EnemyStateApproach.h"
#include"EnemyStateLeave.h"

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
	//初期状態をセット
	TransitionState(std::make_unique<EnemyStateApproach>(this));
}

void Enemy::ApproachInitialize(){
	//タイマーの初期化
	cooltime_ = kShootInterval;
}

void Enemy::Move(){
	worldTransform_.translation_ += velocity_;
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

void Enemy::TransitionState(std::unique_ptr<BaseEnemyState>state){
	state_ = std::move(state);
}


void Enemy::Update(){
	if (isMove){

	}

	//状態ごとの更新処理
	state_->Update();

	//行列の更新
	worldTransform_.UpdateMatrix();

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