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
    /// <summary>
    /// 適用
    /// </summary>
    void ApplyGlobalVariables()override;

private:
    float targetArmAngle_ = 1.3f;
    float shakeUpAngle_ = -3.1f;
    float weaponInitAngle_ = 0.0f;
};

