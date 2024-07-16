#pragma once
#include "PlayerBaseBehavior.h"

class PlayerDashBehavior final:
    public PlayerBaseBehavior{

    PlayerDashBehavior(Player* player);
    ~PlayerDashBehavior()override;

    /// <summary>
    /// 初期化
    /// </summary>
    void Initialize()override;
    /// <summary>
    /// 更新
    /// </summary>
    void Update()override;
};

