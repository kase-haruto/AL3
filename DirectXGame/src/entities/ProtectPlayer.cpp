#include "ProtectPlayer.h"
#include"TextureManager.h"
#include"PlayerStronghold.h"
#include <cmath>

void ProtectPlayer::Initialize(const std::vector<Model*>& model){
	Player::Initialize(model);

	moveRangeModel_.reset(Model::CreateFromOBJ("moveRange", true));
	moveRangeModel_->SetAlpha(0.5f);
	rangeTransform_.Initialize();

}

void ProtectPlayer::InitializeUi(){
	hpSpriteHandle_ = TextureManager::Load("./Resources/hpBar.png");
	hpBar_.reset(Sprite::Create(hpSpriteHandle_, {70.0f,50.0f}, {1, 1, 1, 1}, {0.0f,0.0f}));
	Vector2 size {500.0f,50.0f};
	hpBar_->SetSize(size);

	iconSpriteHandle_ = TextureManager::Load("./Resources/playerProtectIcon.png");
	icon_.reset(Sprite::Create(iconSpriteHandle_, {10,50}));
	Vector2 textureSize {50,50};
	icon_->SetSize(textureSize);
}

void ProtectPlayer::Update(){


	///////////////////////////////////////////////////////////////////////////
	//      移動範囲を制限
	///////////////////////////////////////////////////////////////////////////

	if (strongholdPtr_){
		Vector3 strongholdPosition = strongholdPtr_->GetWorldPosition();
		Vector3 protectPlayerPosition = this->GetWorldPosition();

		// 許容される最大距離
		float maxDistance = 30.0f; // 例として30.0fの距離制限を設定

		// ProtectPlayer と PlayerStronghold との距離を測る
		Vector3 direction = protectPlayerPosition - strongholdPosition;
		float distance = Length(direction);

		// 距離が最大距離を超えている場合、Clampで制限
		if (distance > maxDistance){
			Vector3 clampedPosition = strongholdPosition + direction.Normalize() * maxDistance;
			worldTransform_.translation_ = clampedPosition;
		}
	}

	rangeTransform_.UpdateMatrix();

	//攻撃などの行動の更新
	Player::Update();
}

void ProtectPlayer::Draw(const ViewProjection& viewProjection){
	Player::Draw(viewProjection);
	moveRangeModel_->Draw(rangeTransform_, viewProjection);
}

void ProtectPlayer::OnCollision(Collider* other){
	Player::OnCollision(other);
}

void ProtectPlayer::SetStrongholdPtr(PlayerStronghold* stronghold){
	strongholdPtr_ = stronghold;
	rangeTransform_.translation_ = strongholdPtr_->GetWorldTransform().translation_;
	//地面と被らないように少しだけ浮かす
	rangeTransform_.translation_.y += 0.1f;
}
