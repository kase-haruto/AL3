#include "EnemyBullet.h"
#include"CollisionManager.h"
#include"Player.h"
EnemyBullet::EnemyBullet(){
	//衝突属性
	collisionAttribute_ = 0b1 << 1;;
	//衝突マスク(相手)
	collisionMask_ = ~collisionAttribute_;
	SetCollisionAttribute(collisionAttribute_);//敵陣営
	SetCollisionMask(collisionMask_);//敵陣営以外

	//コライダーリストに追加
	CollisionManager::GetInstance()->SetCollider(this);
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
	float horizontalDistance = sqrtf(vel.x * vel.x + vel.z * vel.z);
	worldTransform_.rotation_.x = std::atan2(-vel.y, horizontalDistance);
	worldTransform_.translation_ = position;
	
	radius_ = 1.0f;
}

void EnemyBullet::HomingInit(float speed){
	speed_ = speed;
}

void EnemyBullet::Update(){
	if (isHoming_){
		Homing();
	}

	worldTransform_.translation_ += velocity_;
	worldTransform_.UpdateMatrix();

	//時間経過でデス
	if (--deathTimer_ <= 0){
		isDead_ = true;
	}
}

void EnemyBullet::Homing(){
	//敵弾から自キャラへのベクトルの取得
	Vector3 toPlayer = player_->GetWorldPosition() - worldTransform_.translation_;
	float t = 0.05f;
	//ベクトルを正規化する
	toPlayer = Normalize(toPlayer);
	velocity_ = Normalize(velocity_);
	velocity_ = Slerp(velocity_, toPlayer, t) * speed_;

	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);
	worldTransform_.rotation_.x = std::atan2(velocity_.y, velocity_.z);
}

void EnemyBullet::Draw(const ViewProjection& viewProjection){
		Bullet::Draw(viewProjection);
}	

Vector3 EnemyBullet::GetWorldPosition()const{
	Vector3 wPos;
	//ワールド行列の平行移動成分を取得
	wPos.x = worldTransform_.matWorld_.m[3][0];
	wPos.y = worldTransform_.matWorld_.m[3][1];
	wPos.z = worldTransform_.matWorld_.m[3][2];
	return wPos;
}

void EnemyBullet::OnCollision(){
	isDead_ = true;
}