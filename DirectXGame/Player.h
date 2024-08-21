#pragma once

#include "Actor.h"
#include "MyFunc.h"
#include "Model.h"
#include "PlayerBaseBehavior.h"
#include "Hammer.h"
#include <vector>
#include <optional>

/// <summary>
/// パーツ/行動
/// </summary>
namespace PlayerDetails{
    /// <summary>
    /// プレイヤーのパーツ
    /// </summary>
    enum class Parts{
        body,
        head,
        L_arm,
        R_arm,
        partsCount
    };

    /// <summary>
    /// プレイヤーの行動状態
    /// </summary>
    enum class Behavior{
        root,   // 通常
        attack, // 攻撃
        dash,   // ダッシュ中
        jump,   // ジャンプ中
    };
}

class LockOn;
class WeaponBase;

using PlayerDetails::Behavior;
using PlayerDetails::Parts;

class Player 
    : public Actor{
public:
    Player();
    ~Player() override;

    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name="model">プレイヤーのモデル配列</param>
    void Initialize(const std::vector<Model*>& model) override;

    /// <summary>
    /// 更新
    /// </summary>
    void Update() override;

    /// <summary>
    /// 描画
    /// </summary>
    /// <param name="viewProjection">ビュー射影行列</param>
    void Draw(const ViewProjection& viewProjection) override;

    /// <summary>
    /// 向いている方向に進む
    /// </summary>
    /// <param name="speed">移動速度</param>
    void MoveInDirection(float speed);

    /// <summary>
    /// 状態の遷移
    /// </summary>
    /// <param name="newState">新しい状態</param>
    void ChangeState(std::unique_ptr<PlayerBaseBehavior> newState);

    /// <summary>
    /// 衝突時の反応
    /// </summary>
    void OnCollision([[maybe_unused]] Collider* other) override;

#pragma region Getter Functions

    bool GetIsAttack() const;
    WorldTransform* GetPartsTransform(int index){ return partsTransform_[index].get(); }
    const WorldTransform& GetWeaponTransform() const{ return weapon_->GetWorldTransform(); }
    WeaponBase* GetWeapon(){ return weapon_; }
    Vector3 GetVelocity() const;
    Vector3 GetDirection() const;
    float GetTargetAngle() const;
    const LockOn* GetLockOn() const;
    bool HasLockOnTarget() const;
    Vector3 GetCenterPos() const override;
    bool GetIsActive() const{ return isActive_; }

#pragma endregion

#pragma region Setter Functions

    void SetViewProjection(const ViewProjection* viewProjection);
    void SetIsAttack(const bool isAttack);
    void SetVelocity(const Vector3& vel);
    void SetDirection(const Vector3& dir);
    void SetHeadTranslation(const Vector3& translation);
    void Set_L_ArmTranslation(const Vector3& translation);
    void Set_R_ArmTranslation(const Vector3& translation);
    void SetBodyTranslation(const Vector3& translation);
    void SetHeadRotation(const Vector3& rotation);
    void Set_L_ArmRotation(const Vector3& rotation);
    void Set_R_ArmRotation(const Vector3& rotation);
    void SetBodyRotation(const Vector3& rotation);
    void SetHeadTranslationX(const float translationX);
    void SetHeadTranslationY(const float translationY);
    void SetHeadTranslationZ(const float translationZ);
    void SetBodyTranslationX(const float translationX);
    void SetBodyTranslationY(const float translationY);
    void SetBodyTranslationZ(const float translationZ);
    void Set_L_ArmTranslationX(const float translationX);
    void Set_L_ArmTranslationY(const float translationY);
    void Set_L_ArmTranslationZ(const float translationZ);
    void Set_R_ArmTranslationX(const float translationX);
    void Set_R_ArmTranslationY(const float translationY);
    void Set_R_ArmTranslationZ(const float translationZ);
    void SetWeaponTranslation(const Vector3& translation);
    void SetWeaponTranslationX(const float translation);
    void SetWeaponTranslationY(const float translation);
    void SetWeaponTranslationZ(const float translation);
    void SetHeadRotationX(const float RotationX);
    void SetHeadRotationY(const float RotationY);
    void SetHeadRotationZ(const float RotationZ);
    void SetBodyRotationX(const float RotationX);
    void SetBodyRotationY(const float RotationY);
    void SetBodyRotationZ(const float RotationZ);
    void Set_L_ArmRotationX(const float RotationX);
    void Set_L_ArmRotationY(const float RotationY);
    void Set_L_ArmRotationZ(const float RotationZ);
    void Set_R_ArmRotationX(const float RotationX);
    void Set_R_ArmRotationY(const float RotationY);
    void Set_R_ArmRotationZ(const float RotationZ);
    void SetWeaponRotation(const Vector3& rotation);
    void SetWeaponRotationX(const float rotation);
    void SetWeaponRotationY(const float rotation);
    void SetWeaponRotationZ(const float rotation);
    void SetWeapon(WeaponBase* weapon);
    void SetLockOn(const LockOn* lockOn);
    void SetIsActive(bool isActive){ isActive_ = isActive; }

#pragma endregion

private:
    /// <summary>
    /// パーツごとの初期化
    /// </summary>
    void PartsTransformInit();

    /// <summary>
    /// 定数の適用
    /// </summary>
    void ApplyGlobalVariables();

private:
    //移動速度
    Vector3 velocity_ = {0.0f,0.0f,0.0f};
    
    //追従カメラ用
    const ViewProjection* viewPorjection_ = nullptr;
    
    //状態
    std::unique_ptr<PlayerBaseBehavior> currentState_;
    
    //パーツごとのトランスフォーム
    std::vector<std::unique_ptr<WorldTransform>> partsTransform_;
   
    //攻撃しているか
    bool isAttack_ = false;

    //旋回する際の目標角度
    float targetAngle = 0.0f;

    //ロックオン
    const LockOn* lockOn_ = nullptr;

    //武器
    WeaponBase* weapon_ = nullptr;

    //操作可能かどうか
    bool isActive_ = false;
};
