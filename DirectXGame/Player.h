#pragma once
#include "Actor.h"
#include "MyFunc.h"
#include "Model.h"
#include "PlayerBaseBehavior.h"
#include <vector>
#include <optional>

namespace PlayerDetails{
    /// <summary>
    /// プレイヤーのパーツ
    /// </summary>
    enum class Parts{
        body,
        head,
        L_arm,
        R_arm,
        weapon,
        partsCount
    };

    enum class Behavior{
        root,   // 通常
        attack, // 攻撃
        dash,   // ダッシュ中
        jump,   // ジャンプ中
    };
}

using PlayerDetails::Behavior;
using PlayerDetails::Parts;

class Player : public Actor{
public:
    Player();
    ~Player() override;

    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name="model"></param>
    void Initialize(const std::vector<Model*>& model) override;
    /// <summary>
    /// 更新
    /// </summary>
    void Update() override;
    /// <summary>
    /// 描画
    /// </summary>
    /// <param name="viewProjection"></param>
    void Draw(const ViewProjection& viewProjection) override;
    /// <summary>
    /// 向いている方向に進む
    /// </summary>
    /// <param name="speed"></param>
    void MoveInDirection(float speed);

    void SetViewProjection(const ViewProjection* viewProjection);
    void ChangeState(std::unique_ptr<PlayerBaseBehavior> newState);

#pragma
    bool GetIsAttack() const;
    WorldTransform* GetPartsTransform(int index){ return partsTransform_[index].get(); }
    Vector3 GetVelocity() const;
    Vector3 GetDirection() const;
    std::optional<Behavior> GetBehaviorRequest() const;
    float GetTargetAngle() const;
#pragma endregion

#pragma region

    void SetIsAttack(const bool isAttack);
    void SetVelocity(const Vector3& vel);
    void SetDirection(const Vector3& dir);
    void SetBehavior(Behavior behavior);

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
#pragma endregion セッター

private:
    void PartsTransformInit();
    void TrasitionaBehavior();
    void ApplyGlobalVariables();

private:
    Vector3 velocity_ = {0.0f,0.0f,0.0f};
    const ViewProjection* viewPorjection_ = nullptr;
    Behavior behavior_ = Behavior::root;
    std::optional<Behavior> behaviorRequest_ = std::nullopt;
    std::unique_ptr<PlayerBaseBehavior> currentState_;
    std::vector<std::unique_ptr<WorldTransform>> partsTransform_;
    bool isAttack_ = false;
    float targetAngle = 0.0f;
};
