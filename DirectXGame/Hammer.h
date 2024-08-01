#pragma once
#include "Collider.h"
#include"ViewProjection.h"
#include"WeaponBase.h"
/// <summary>
/// ハンマー
/// </summary>
class Hammer :
    public WeaponBase{
public:
    Hammer() = default;
    ~Hammer()override = default;

    void Initialize()override;
    void Draw(const ViewProjection& viewProjection)override;

private:
    
};

