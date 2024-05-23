#include "Player.h"
#include <algorithm>
#include"MyFunc.h"
#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG
#include"CollisionManager.h"


Player::Player(){
	//衝突属性
	collisionAttribute_ = 0b1;
	//衝突マスク(相手)
	collisionMask_ = ~collisionAttribute_;
	SetCollisionAttribute(collisionAttribute_);//敵陣営
	SetCollisionMask(collisionMask_);//敵陣営以外

	//コライダーリストに追加
	CollisionManager::GetInstance()->SetCollider(this);
}

Player::~Player(){}

void Player::Init(Model* model,Vector3 pos){
	assert(model);
	model_ = model;
	textuerHandle_ = TextureManager::Load("./Resources/uvChecker.png");
	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;
	velocity_ = {0.4f, 0.4f, 0.4f};
	radius_ = 1.0f;

	input_ = Input::GetInstance();
	
	
}

void Player::Shoot(){
	if (input_->TriggerKey(DIK_SPACE)){
		// 自キャラの座標をコピー
		Vector3 pos = worldTransform_.translation_;

		// 弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 BulletVel = {0, 0, kBulletSpeed};

		// 速度ベクトルを自キャラの向きに合わせて回転
		BulletVel = TransformNormal(BulletVel, this->worldTransform_.matWorld_);

		// 弾を生成してユニークポインタにラップ
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, GetWorldPosition(), BulletVel);

		// 弾を登録
		bullets_.push_back(std::move(newBullet));
	}

	
}

void Player::Move(){
	Vector3 move = {0, 0, 0};

	if (input_->PushKey(DIK_A)){
		move.x -= velocity_.x;
	} else if (input_->PushKey(DIK_D)){
		move.x += velocity_.x;
	}

	if (input_->PushKey(DIK_S)){
		move.y -= velocity_.y;
	} else if (input_->PushKey(DIK_W)){
		move.y += velocity_.y;
	}

	worldTransform_.translation_ += move;

	//移動限界座標
	const float kMoveLimitX = 32.0f;
	const float kMoveLimitY = 16.0f;

	worldTransform_.translation_.x = std::clamp(worldTransform_.translation_.x, -kMoveLimitX, kMoveLimitX);
	worldTransform_.translation_.y = std::clamp(worldTransform_.translation_.y, -kMoveLimitY, kMoveLimitY);

	worldTransform_.UpdateMatrix();
}

void Player::Rotate(){
	//回転の速さ
	const float kRotSpeed = 0.02f;
	if (input_->PushKey(DIK_RIGHT)){
		worldTransform_.rotation_.y += kRotSpeed;
	} else if (input_->PushKey(DIK_LEFT)){
		worldTransform_.rotation_.y -= kRotSpeed;
	}
}

void Player::Update(){

	Move();
	Shoot();
	Rotate();

	// 弾の更新
	for (auto& bullet : bullets_){
		bullet->Update();
	}

	// 生存フラグがfalseの弾を削除
	bullets_.remove_if([] (const std::unique_ptr<PlayerBullet>& bullet){
		if (bullet->GetIsDead()){
			//衝突判定リストから削除
			CollisionManager::GetInstance()->RemoveCollider(bullet.get());
			return true;
		}
		return false;
					   });


#ifdef _DEBUG
	ImGui::Begin("player");
	ImGui::DragFloat3("translate", &worldTransform_.translation_.x, 0.01f);
	ImGui::End();
#endif // _DEBUG
}

void Player::Draw(ViewProjection& viewprojection){
	Actor::Draw(viewprojection);
	for (auto& bullet : bullets_){
		bullet->Draw(viewprojection);
	}
}

Vector3 Player::GetWorldPosition()const{
	Vector3 wPos;
	//ワールド行列の平行移動成分を取得
	wPos.x = worldTransform_.matWorld_.m[3][0];
	wPos.y = worldTransform_.matWorld_.m[3][1];
	wPos.z = worldTransform_.matWorld_.m[3][2];
	return wPos;
}

void Player::OnCollision(){

}

void Player::SetParent(const WorldTransform* parent){
	//親子関係を結ぶ
	worldTransform_.parent_ = parent;
}