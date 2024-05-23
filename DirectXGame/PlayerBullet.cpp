#include "PlayerBullet.h"
#include"CollisionManager.h"

PlayerBullet::PlayerBullet(){
	//衝突属性
	collisionAttribute_ = 0b1;
	//衝突マスク(相手)
	collisionMask_ = ~collisionAttribute_;
	SetCollisionAttribute(collisionAttribute_);//敵陣営
	SetCollisionMask(collisionMask_);//敵陣営以外

	//コライダーリストに追加
	CollisionManager::GetInstance()->SetCollider(this);
}

PlayerBullet::~PlayerBullet(){}

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity){
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	textuerHandle_ = TextureManager::Load("./Resources/white1x1.png");
	velocity_ = velocity;
	radius_ = 1.0f;	
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
	//ワールド行列の平行移動成分を取得
	wPos.x = worldTransform_.matWorld_.m[3][0];
	wPos.y = worldTransform_.matWorld_.m[3][1];
	wPos.z = worldTransform_.matWorld_.m[3][2];
	return wPos;
}