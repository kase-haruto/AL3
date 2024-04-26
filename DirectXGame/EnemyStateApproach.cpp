#include "EnemyStateApproach.h"
#include"Enemy.h"

EnemyStateApproach::EnemyStateApproach(Enemy* enemy)
	:BaseEnemyState("State Approach", enemy){

}

EnemyStateApproach::~EnemyStateApproach(){}

void EnemyStateApproach::Update(){
	enemy_->SetVelocity({0.0f, 0.0f, -0.2f});

	//移動
	enemy_->Move();

	if (enemy_->GetTranslate().z <= 10){
		enemy_->TransitionState(std::make_unique<EnemyStateLeave>(enemy_));
		return;
	}

	

	// クールタイムが0になったら発射
	if (enemy_->GetCoolTime() <= 0){
		enemy_->Shoot();
		// クールタイムの初期化
		enemy_->ApproachInitialize();
	} else{
		// 弾を撃つクールタイムを減少
		enemy_->SetCoolTime(enemy_->GetCoolTime() - 1);
	}
}