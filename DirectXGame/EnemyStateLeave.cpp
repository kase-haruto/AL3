#include "EnemyStateLeave.h"
#include"Enemy.h"

EnemyStateLeave::EnemyStateLeave(Enemy* enemy)
	:BaseEnemyState("State Leave", enemy){

}

EnemyStateLeave::~EnemyStateLeave(){}

void EnemyStateLeave::Update(){
	enemy_->SetVelocity({0.2f, 0.2f, 0.0f});

	//移動
	enemy_->Move();
}