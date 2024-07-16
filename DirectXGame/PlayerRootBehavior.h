#pragma once
#include "PlayerBaseBehavior.h"

class PlayerRootBehavior final :
    public PlayerBaseBehavior{
public:
    PlayerRootBehavior(Player* player);
    ~PlayerRootBehavior()override;

    /// <summary>
    /// 初期化
    /// </summary>
    void Initialize()override;
    /// <summary>
    /// 更新
    /// </summary>
    void Update()override;
private:
    /// <summary>
    /// 移動
    /// </summary>
    void Move();
    /// <summary>
    /// 浮遊行動の初期化
    /// </summary>
    void InitializeFloatingAction();
    /// <summary>
    /// 浮遊行動の更新
    /// </summary>
    void UpdateFloatingAction();

private:
    float floatingParameter_;
    float floatingAmplitude_;
    float cycle_;
};

