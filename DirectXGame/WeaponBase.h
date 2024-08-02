#pragma once
#include "Collider.h"
#include"ViewProjection.h"

/// <summary>
/// ハンマー
/// </summary>
class WeaponBase:
    public Collider{
public:
    WeaponBase() = default;
    virtual ~WeaponBase()override = default;

    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name="model"></param>
    virtual void Initialize(Model* model) = 0;
    /// <summary>
    /// 描画
    /// </summary>
    /// <param name="viewProjection"></param>
    virtual void Draw(const ViewProjection& viewProjection) = 0;
    /// <summary>
    /// 衝突時の反応
    /// </summary>
    virtual void OnCollision([[maybe_unused]] Collider* other)override = 0;


    /// <summary>
    /// 中心座標
    /// </summary>
    /// <returns></returns>
    virtual Vector3 GetCenterPos()const override =0;



    void SetModel(Model* model);

protected:
    Model* model_;
};