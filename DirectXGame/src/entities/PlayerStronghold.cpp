#include "PlayerStronghold.h"

void PlayerStronghold::Initialize(Model* model){
	BaseStronghold::Initialize(model);
}

void PlayerStronghold::Update(){
	ExtinctionCharacter();
	domeTransform_.translation_ = worldTransform_.translation_;
	worldTransform_.UpdateMatrix();
}

void PlayerStronghold::Draw(const ViewProjection& viewProjection){
	BaseStronghold::Draw(viewProjection);

		BaseStronghold::DrawDome(viewProjection);
}

void PlayerStronghold::ExtinctionCharacter(){
	if (!protectPlayer_->GetIsAlive()){
		// 線形補完を使ってサイズを徐々に小さくする

		domeTransform_.scale_ = Lerp(domeTransform_.scale_, {0.0f, 0.0f, 0.0f}, 0.04f);

	}
	domeTransform_.UpdateMatrix();
}
