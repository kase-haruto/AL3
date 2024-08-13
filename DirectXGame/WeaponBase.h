#pragma once
#include "Collider.h"
#include"ViewProjection.h"
#include"ContactRecord.h"


class Actor;

/// <summary>
/// ハンマー
/// </summary>
class WeaponBase :
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
    /// 更新
    /// </summary>
    virtual void Update() = 0;

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
    /// 接触履歴のクリア
    /// </summary>
    void ClearContactRecord();


    /// <summary>
    /// 中心座標
    /// </summary>
    /// <returns></returns>
    virtual Vector3 GetCenterPos()const override = 0;

    const WorldTransform& GetWorldTransform()const{ return worldTransform_; }
    WorldTransform& GetWorldTransform(){ return worldTransform_; }
    const Vector3& GetRotation()const;
    const Vector3& GetTranslation()const;

    void SetRotation(const Vector3& rotation);
    void SetRotationX(const float rotation);
    void SetRotationY(const float rotation);
    void SetRotationZ(const float rotation);

    void SetTranslation(const Vector3& translation);
    void SetTranslationX(const float translation);
    void SetTranslationY(const float translation);
    void SetTranslationZ(const float translation);

    void SetModel(Model* model);
    void SetParent(WorldTransform worldTransform);

protected:
    Model* model_;
    WorldTransform worldTransform_;

    //敵のシリアルナンバー記録
    ContactRecord contactRecord_;
};