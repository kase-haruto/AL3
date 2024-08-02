#pragma once
#include<cstdint>

enum class CollisionTypeIdDef:uint32_t{
	kDefault,			//デフォルト
	kPlayer,			//プレイヤー
	kPlayerWeapon,		//武器
	kEnemy,				//敵
	kObstacleOBJ,		//障害物
};

