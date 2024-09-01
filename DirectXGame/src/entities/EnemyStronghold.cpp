#include "EnemyStronghold.h"
#include"EnemyManager.h"


void EnemyStronghold::Initialize(Model* model){
	//モデルを代入/worldTransformの初期化
	BaseStronghold::Initialize(model);
}

void EnemyStronghold::Update(){
	ExtinctionCharacter();
	domeTransform_.translation_ = worldTransform_.translation_;
	worldTransform_.UpdateMatrix();
}

void EnemyStronghold::Draw(const ViewProjection& viewProjection){
	BaseStronghold::Draw(viewProjection);

	BaseStronghold::DrawDome(viewProjection);
}

void EnemyStronghold::SetEnemyManager(const std::unique_ptr<EnemyManager>& manager){
	enemyManager_ = manager.get();
}

void EnemyStronghold::SetPlayerPtr(const std::unique_ptr<AttackPlayer>& player){
	playerPtr_ = player.get();
}

void EnemyStronghold::SetProtectEnemy(ProtectEnemy* enemy){
	protectEnemy_ = enemy;
}

void EnemyStronghold::ExtinctionCharacter(){
	if (!protectEnemy_->GetIsAlive()){
		// 線形補完を使ってサイズを徐々に小さくする

		domeTransform_.scale_ = Lerp(domeTransform_.scale_, {0.0f, 0.0f, 0.0f}, 0.04f);

	}
	domeTransform_.UpdateMatrix();
}
