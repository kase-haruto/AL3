#pragma once
#include "PlayerBaseBehavior.h"
#include "PlayerAttackCommand.h"

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

    void AddCommand(std::unique_ptr<PlayerAttackCommand> command);
private:
    std::vector<std::unique_ptr<PlayerAttackCommand>>attackCommands_;
    int currentCommandIndex_;
    uint32_t comboReceptionTime_ = 30;//1秒
    const int comboReceptionTimeMax_ = 30;
    
};

