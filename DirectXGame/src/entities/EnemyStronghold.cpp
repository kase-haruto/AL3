#include "EnemyStronghold.h"
#include"EnemyManager.h"


void EnemyStronghold::Initialize(Model* model){
	//モデルを代入/worldTransformの初期化
	BaseStronghold::Initialize(model);
}

void EnemyStronghold::Update(){


	worldTransform_.UpdateMatrix();
}

void EnemyStronghold::Draw(const ViewProjection& viewProjection){
	BaseStronghold::Draw(viewProjection);
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
