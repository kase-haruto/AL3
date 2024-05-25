#include "Enemy.h"
#include<cassert>
#include"EnemyStateApproach.h"
#include"EnemyStateLeave.h"
#include"Player.h"
#include"MyFunc.h"
#include"CollisionManager.h"
#include"GameScene.h"

Enemy::Enemy(){
	//衝突属性
	collisionAttribute_ = 0b1 << 1;
	//衝突マスク(相手)
	collisionMask_ = ~collisionAttribute_;
	SetCollisionAttribute(collisionAttribute_);//敵陣営
	SetCollisionMask(collisionMask_);//敵陣営以外

	//コライダーリストに追加
	CollisionManager::GetInstance()->SetCollider(this);
}

Enemy::~Enemy(){

}

void Enemy::Init(Vector3 pos,Model* model, GameScene* gameScene){
	assert(gameScene);
	assert(model);
	gameScene_ = gameScene;
	model_ = model;
	textuerHandle_ = TextureManager::Load("./Resources/cat.png");
	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;
	radius_ = 1.0f;

	currentPhase = Phase::Approach;
	ApproachInitialize();
	//初期状態をセット
	TransitionState(std::make_unique<EnemyStateApproach>(this));

}

void Enemy::Approach2Leave(){
	timedCalls_.clear();
	currentPhase = Phase::Leave;
}

void Enemy::ApproachInitialize(){
	//タイマーの初期化
	ShootAndLisetTimer();
}

void Enemy::Move(){
	worldTransform_.translation_ += velocity_;
}

void Enemy::TransitionState(std::unique_ptr<BaseEnemyState>state){
	state_ = std::move(state);
}

void Enemy::Shoot(){
	assert(player_);

	const float kBulletSpeed = 1.0f;
	Vector3 bulletVel;

	//自キャラのワールド座標を取得する
	Vector3 playerWPos = player_->GetWorldPosition();
	//enemyのワールド座標
	Vector3 wPos = GetWorldPosition();
	//敵キャラと自キャラの差分ベクトルを求める
	Vector3 differenceV = playerWPos - wPos;
	differenceV = Normalize(differenceV);
	bulletVel = differenceV * kBulletSpeed;


	//速度ベクトルを自キャラの向きに合わせて回転
	bulletVel = TransformNormal(bulletVel, this->worldTransform_.matWorld_);

	// 弾を生成してユニークポインタにラップ
	//EnemyBullet* newBullet = new EnemyBullet;
	auto newBullet = std::make_unique<EnemyBullet>();
	newBullet->Init(model_, wPos, bulletVel);
	newBullet->SetPlayer(player_);
	newBullet->HomingInit(kBulletSpeed);

	// 弾を登録
	gameScene_->SetEnemyBullet(std::move(newBullet));
}

void Enemy::ShootAndLisetTimer(){
	if (currentPhase == Phase::Approach){
		Shoot();
		//発射タイマーのセット
		timedCalls_.push_back(
			std::make_unique<TimedCall>(std::bind(&Enemy::ShootAndLisetTimer, this), kShootInterval));
	}
}

void Enemy::Update(){
	if (isMove){
		//状態ごとの更新処理
		state_->Update();
	}

	//行列の更新
	worldTransform_.UpdateMatrix();

	for (auto& timedCall : timedCalls_){
		timedCall->Update();
	}
}

void Enemy::OnCollision(){

}

void Enemy::Draw(ViewProjection& viewProjection){
	Actor::Draw(viewProjection);
}

Vector3 Enemy::GetWorldPosition()const{
	Vector3 wPos;
	//ワールド行列の平行移動成分を取得
	wPos.x = worldTransform_.matWorld_.m[3][0];
	wPos.y = worldTransform_.matWorld_.m[3][1];
	wPos.z = worldTransform_.matWorld_.m[3][2];
	return wPos;
}