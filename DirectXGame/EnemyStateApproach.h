#pragma once
#include "BaseEnemyState.h"
#include"EnemyStateLeave.h"

/// <summary>
/// 接近フェーズ
/// </summary>
class EnemyStateApproach :
    public BaseEnemyState{
public:
    //コンストラクタ
    EnemyStateApproach(Enemy* enemy);
    ~EnemyStateApproach()override;

    //更新
    void Update()override;
};

