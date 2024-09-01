#pragma once

#include "Actor.h"
#include "MyFunc.h"
#include "Model.h"
#include "PlayerBaseBehavior.h"
#include "Hammer.h"
#include <vector>
#include <optional>
#include"Sprite.h"


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
class Enemy;

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
    /// uiの初期化
    /// </summary>
    virtual void InitializeUi();

    /// <summary>
    /// 更新
    /// </summary>
    void Update() override;

    void UpdateHPBar();

    void UpdateTransform();

    /// <summary>
    /// 描画
    /// </summary>
    /// <param name="viewProjection">ビュー射影行列</param>
    void Draw(const ViewProjection& viewProjection) override;

    /// <summary>
    /// spriteの描画
    /// </summary>
    void DrawUiSprite();

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

    /// <summary>
    /// ライフの最大値
    /// </summary>
    /// <param name="max">初期化後に使用</param>
    void SetMaxLife(int max);

    // 敵のリストを設定するメソッド
    void SetEnemyLists(const std::list<std::unique_ptr<Enemy>>& allEnemies);

    /// <summary>
    /// 操作されていない時の自動の動き
    /// </summary>
    void UpdateAutoMode();

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
    // 各種セッター
    void SetPartTranslation(Parts part, const Vector3& translation);
    void SetPartRotation(Parts part, const Vector3& rotation);
    void SetPartTranslationX(Parts part, float translationX);
    void SetPartTranslationY(Parts part, float translationY);
    void SetPartTranslationZ(Parts part, float translationZ);
    void SetPartRotationX(Parts part, float rotationX);
    void SetPartRotationY(Parts part, float rotationY);
    void SetPartRotationZ(Parts part, float rotationZ);

    // 個別のパーツセッター
    void SetHeadTranslation(const Vector3& translation);
    void SetHeadRotation(const Vector3& rotation);
    void SetHeadTranslationX(float translationX);
    void SetHeadTranslationY(float translationY);
    void SetHeadTranslationZ(float translationZ);
    void SetHeadRotationX(float rotationX);
    void SetHeadRotationY(float rotationY);
    void SetHeadRotationZ(float rotationZ);

    void SetBodyTranslation(const Vector3& translation);
    void SetBodyRotation(const Vector3& rotation);
    void SetBodyTranslationX(float translationX);
    void SetBodyTranslationY(float translationY);
    void SetBodyTranslationZ(float translationZ);
    void SetBodyRotationX(float rotationX);
    void SetBodyRotationY(float rotationY);
    void SetBodyRotationZ(float rotationZ);

    void Set_L_ArmTranslation(const Vector3& translation);
    void Set_L_ArmRotation(const Vector3& rotation);
    void Set_L_ArmTranslationX(float translationX);
    void Set_L_ArmTranslationY(float translationY);
    void Set_L_ArmTranslationZ(float translationZ);
    void Set_L_ArmRotationX(float rotationX);
    void Set_L_ArmRotationY(float rotationY);
    void Set_L_ArmRotationZ(float rotationZ);

    void Set_R_ArmTranslation(const Vector3& translation);
    void Set_R_ArmRotation(const Vector3& rotation);
    void Set_R_ArmTranslationX(float translationX);
    void Set_R_ArmTranslationY(float translationY);
    void Set_R_ArmTranslationZ(float translationZ);
    void Set_R_ArmRotationX(float rotationX);
    void Set_R_ArmRotationY(float rotationY);
    void Set_R_ArmRotationZ(float rotationZ);

    void SetWeaponTranslation(const Vector3& translation);
    void SetWeaponTranslationX(float translationX);
    void SetWeaponTranslationY(float translationY);
    void SetWeaponTranslationZ(float translationZ);
    void SetWeaponRotation(const Vector3& rotation);
    void SetWeaponRotationX(float rotationX);
    void SetWeaponRotationY(float rotationY);
    void SetWeaponRotationZ(float rotationZ);

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
    int maxLife_;

    //移動速度
    Vector3 velocity_ = {0.0f,0.0f,0.0f};
    
    //追従カメラ用
    const ViewProjection* viewPorjection_ = nullptr;
    
    //状態
    std::unique_ptr<PlayerBaseBehavior> currentState_;
    
    //パーツごとのトランスフォーム
    std::vector<std::unique_ptr<WorldTransform>> partsTransform_;
   

    //旋回する際の目標角度
    float targetAngle = 0.0f;

    //ロックオン
    const LockOn* lockOn_ = nullptr;

    //武器
    WeaponBase* weapon_ = nullptr;

   


protected:

    //hpバー
    std::unique_ptr<Sprite> hpBar_ = nullptr;

    //アイコン
    std::unique_ptr<Sprite> icon_ = nullptr;
  
    //テクスチャハンドル
    uint32_t hpSpriteHandle_;

    uint32_t iconSpriteHandle_;

    //操作可能かどうか
    bool isActive_ = false;

    //攻撃しているか
    bool isAttack_ = false;


    //攻撃用クールタイム
    int coolTime_ = 420; //7秒ごとに自動で攻撃をするようにする

    const int coolTimeLimit_ = 420;

    // 敵のリスト
    std::list<Enemy*> enemies_;

    //サウンドデータ
    uint32_t hitSoundHandle_ = 0;
    uint32_t hitVoiceHandle_ = 0;
};
