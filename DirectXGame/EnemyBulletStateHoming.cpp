#include "EnemyBulletStateHoming.h"
#include"EnemyBullet.h"
EnemyBulletStateHoming::EnemyBulletStateHoming(EnemyBullet* bullet)
	:BaseEnemyBulletState("homing", bullet){

}


void Enemy