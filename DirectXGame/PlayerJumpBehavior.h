#pragma once
#include "PlayerBaseBehavior.h"
#include"Vector3.h"
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
    /// <summary>
    /// 適用
    /// </summary>
    void ApplyGlobalVariables()override;

private:
    //ジャンプの初速
    Vector3 jumpFirstSpeed_ {0.0f,1.0f,0.0f};
    //重力加速度
    float kGravityAcceleration_ = 0.05f;
};

