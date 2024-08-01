#pragma once
#include "Collider.h"
#include"ViewProjection.h"
#include"WeaponBase.h"
/// <summary>
/// ハンマー
/// </summary>
class Hammer final:
    public WeaponBase{
public:
    Hammer() = default;
    ~Hammer()override = default;

    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name="model"></param>
    void Initialize(Model* model)override;

    /// <summary>
    /// 描画
    /// </summary>
    /// <param name="viewProjection"></param>
    void Draw(const ViewProjection& viewProjection)override;
   
    /// <summary>
    /// 衝突時の反応
    /// </summary>
    void OnCollision()override;

    /// <summary>
    /// 中心座標
    /// </summary>
    /// <returns></returns>
    Vector3 GetCenterPos()const override;

private:
    
};

