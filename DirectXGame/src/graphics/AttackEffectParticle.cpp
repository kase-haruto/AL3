#include "AttackEffectParticle.h"
#include<random>

AttackEffectParticle::AttackEffectParticle():target_(nullptr),effectDuration_(0.5f){//1秒感表示
	model_.reset(Model::CreateFromOBJ("attackEffect", true));
}


void AttackEffectParticle::Initialize(){
	worldTransform_.Initialize();
	if (target_){
		Vector3 offsetPos = {1.0f, 0.0f, 0.0f};
		worldTransform_.translation_ = target_->translation_ + offsetPos;
		startTime_ = std::chrono::steady_clock::now(); // 開始時間を記録
	}
}

void AttackEffectParticle::Initialize(const WorldTransform* target){
	// ランダムデバイスを生成
	std::random_device rd;

	// メルセンヌ・ツイスタ乱数エンジンを初期化
	std::mt19937 gen(rd());

	// 各軸の回転角度に対する一様分布を定義
	std::uniform_real_distribution<float> rotateDist(0.0f, 5.0f);

	// WorldTransformのインスタンスを生成
	worldTransform_.Initialize();

	// スケールを設定
	worldTransform_.scale_ = {3.0f, 3.0f, 3.0f};

	// ランダムな回転角度を生成して設定
	worldTransform_.rotation_ = {rotateDist(gen), rotateDist(gen), rotateDist(gen)};

	if (target){
		target_ = target;
		worldTransform_.translation_ = target_->translation_;
		worldTransform_.UpdateMatrix();
		startTime_ = std::chrono::steady_clock::now(); // 開始時間を記録
	}
}

void AttackEffectParticle::PlayEffect(){

}

void AttackEffectParticle::Update(){
	if (target_){
		// ターゲットの現在の位置にエフェクトを追従させる
		Vector3 offsetPos = {0.0f,1.0f,0.0f};
		worldTransform_.translation_ = target_->translation_ + offsetPos;
		worldTransform_.UpdateMatrix();

		auto currentTime = std::chrono::steady_clock::now();
		std::chrono::duration<float> elapsed = currentTime - startTime_;
		if (elapsed.count() > effectDuration_){
			target_ = nullptr; // 一定時間経過後にターゲットをリセット
		}
	}
}

void AttackEffectParticle::Draw(const ViewProjection& viewProjection){
	if (target_){
		model_->Draw(worldTransform_, viewProjection);
	}
}

void AttackEffectParticle::SetTarget(const WorldTransform* target){ target_ = target; }

bool AttackEffectParticle::IsEffectFinished()const{
	return target_ == nullptr;
}