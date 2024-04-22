#include "Player.h"
#include <algorithm>
#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG



Player::Player(){

}

Player::~Player(){
	for (PlayerBullet* bullet : bullets_){
		delete bullet;
	}
}

/// <summary>
/// 初期化を行います
/// </summary>
void Player::Init(Model* model){
	assert(model);
	model_ = model;
	textuerHandle_ = TextureManager::Load("./Resources/uvChecker.png");
	worldTransform_.Initialize();
	worldTransform_.translation_ = {0.0f,0.0f,0.0f};
	velocity_ = {0.4f, 0.4f, 0.4f};

	input_ = Input::GetInstance();
}

void Player::Shoot(){
	if (input_->TriggerKey(DIK_SPACE)){
		//自キャラの座標をコピー
		Vector3 pos = worldTransform_.translation_;

		//弾を生成
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, pos);

		//弾を登録
		bullets_.push_back(newBullet);
	}
}


/// <summary>
/// 移動を行います
/// </summary>
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

#ifdef _DEBUG
	ImGui::Begin("player");
	ImGui::DragFloat3("translate", &worldTransform_.translation_.x, 0.01f);
	ImGui::End();
#endif // _DEBUG

	Rotate();
	Move();
	Shoot();

	//弾更新
	for (PlayerBullet* bullet : bullets_){
		bullet->Update();
	}

}

/// <summary>
/// 描画処理を行
/// </summary>
void Player::Draw(ViewProjection& viewprojection){
	Actor::Draw(viewprojection);
	for (PlayerBullet* bullet : bullets_){
		bullet->Draw(viewprojection);
	}
}