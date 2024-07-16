#pragma once
#include "PlayerBaseBehavior.h"

class PlayerAttackBehavior final:
    public PlayerBaseBehavior{

public:
    PlayerAttackBehavior(Player* player);
    ~PlayerAttackBehavior()override;

    /// <summary>
    /// 初期化
    /// </summary>
    void Initialize()override;
    /// <summary>
    /// 更新
    /// </summary>
    void Update()override;
};

