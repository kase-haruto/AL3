#pragma once
#include "PlayerAttackCommand.h"
class PlayerSpinAttack :
    public PlayerAttackCommand{
public:
    ~PlayerSpinAttack(){}
    void Initialize(Player* player)override;
    void  Execute(Player* player)override;

private:

};

