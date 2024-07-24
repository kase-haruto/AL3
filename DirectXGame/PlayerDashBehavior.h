#pragma once
#include "PlayerBaseBehavior.h"
#include<stdint.h>

class PlayerDashBehavior final:
    public PlayerBaseBehavior{
public:
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
    /// <summary>
    /// 調整用変数の適用
    /// </summary>
    void ApplyGlobalVariables();
private:
    //ダッシュ用の媒介変数
    int32_t dashParameter_ = 0;

    // ダッシュの速さ
    float dashSpeed = 0.6f;
    //ダッシュ時間
    int32_t dashTime = 30;
};

