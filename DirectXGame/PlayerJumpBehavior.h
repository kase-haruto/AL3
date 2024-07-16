#pragma once
#include "PlayerBaseBehavior.h"
class PlayerJumpBehavior final:
    public PlayerBaseBehavior{
public:
    PlayerJumpBehavior(Player* player);
    ~PlayerJumpBehavior()override;

    /// <summary>
    /// 初期化
    /// </summary>
    void Initialize()override;
    /// <summary>
    /// 更新
    /// </summary>
    void Update()override;
};

