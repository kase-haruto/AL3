#pragma once
#include <memory>

class Player;

class PlayerBaseBehavior{
public:
    PlayerBaseBehavior(Player* player) : player_(player){}
    virtual ~PlayerBaseBehavior() = default;

    virtual void Initialize() = 0;
    virtual void Update() = 0;
    virtual void ApplyGlobalVariables() = 0;

protected:
    Player* player_;
};
