#include "Enemy.h"
#include"CollisionTypeIdDef.h"

#include<numbers>

uint32_t Enemy::nextSerialNumber = 0;

Enemy::Enemy(){
	//シリアルナンバーを振る
	serialNumber_ = nextSerialNumber;
	++nextSerialNumber;

	//パーツの要素数
	partsTransform_.resize(static_cast< int >(Parts::partsCount));
	//パーツごとのtransform
	partsTransform_[static_cast< int >(Parts::body)] = std::make_unique<WorldTransform>();
	partsTransform_[static_cast< int >(Parts::arm)] = std::make_unique<WorldTransform>();
}

Enemy::~Enemy(){}

void Enemy::Initialize(const std::vector<Model*>& models){
	Actor::Initialize(models);

	//パーツごとのトランスフォームの初期化
	for (int i = 0; i < ( int ) Parts::partsCount; ++i){
		partsTransform_[i]->Initialize();
	}

	partsTransform_[static_cast< int >(Parts::body)]->parent_ = &worldTransform_;
	partsTransform_[static_cast< int >(Parts::arm)]->parent_ = partsTransform_[static_cast< int >(Parts::body)].get();

	//衝突判定のid設定
	Collider::SetTypeID(static_cast< uint32_t >(CollisionTypeIdDef::kEnemy));
}

void Enemy::Update(){
	Move();
	ArmWave();
	worldTransform_.UpdateMatrix();

	for (const auto& transform : partsTransform_){
		transform->UpdateMatrix();
	}
}

void Enemy::ArmWave(){
	//浮遊移動のサイクル
	const uint16_t cycle = 60;
	//１フレームでのパラメータ加算値
	const float step = 2.0f * ( float ) std::numbers::pi / cycle;
	//パラメータ1ステップ分加算
	waveParameter_ += step;
	//2piを超えたら0に戻す
	waveParameter_ = std::fmod(waveParameter_, 2.0f * ( float ) std::numbers::pi);
	//浮遊の振幅
	const float floatingAmplitude = 0.2f;
	//浮遊を座標に反映
	partsTransform_[static_cast< int >(Parts::arm)]->translation_.y = std::sin(waveParameter_) * floatingAmplitude;
	partsTransform_[static_cast< int >(Parts::arm)]->translation_.z = std::cos(waveParameter_) * floatingAmplitude;
}

void Enemy::Draw(const ViewProjection& viewProjection){
	models_[static_cast< int >(Parts::body)]->Draw(*partsTransform_[static_cast< int >(Parts::body)], viewProjection);
	models_[static_cast< int >(Parts::arm)]->Draw(*partsTransform_[static_cast<int>(Parts::arm)], viewProjection);
}

void Enemy::Move(){
	//回転速度と移動速度
	const float rotateSpeed= 0.03f;
	const float moveSpeed = 0.3f;
	
	//回転の更新
	worldTransform_.rotation_.y += rotateSpeed;

	Matrix4x4 matRotateY = Matrix4x4::MakeRotateYMatrix(worldTransform_.rotation_.y);

	// 回転行列から前方向ベクトルを計算
	Vector3 forward = Vector3(-matRotateY.m[2][0], -matRotateY.m[2][1], -matRotateY.m[2][2]);

	// 前方向に移動
	worldTransform_.translation_.x += forward.x * moveSpeed;
	worldTransform_.translation_.z += forward.z * moveSpeed;
}

void Enemy::OnCollision([[maybe_unused]] Collider* other){

}

Vector3 Enemy::GetCenterPos()const{
	const Vector3 offset = {0.0f,1.0f,0.0f};

	Vector3 worldPos = Matrix4x4::Transform(offset, worldTransform_.matWorld_);
	return worldPos;
}

