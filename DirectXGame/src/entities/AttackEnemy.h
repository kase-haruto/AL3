#pragma once

#include "Enemy.h"
#include "Player.h"
#include "Collider.h"
#include"PlayerStronghold.h"
#include <memory>

class AttackEnemy : public Enemy{
    /// <summary>
    /// 敵のパーツ
    /// </summary>
    enum class Parts{
        body,
        arm,
        partsCount
    };

public:
    /// <summary>
    /// コンストラクタ/デストラクタ
    /// </summary>
    AttackEnemy() = default;
    ~AttackEnemy() override = default;

    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name="models"></param>
    void Initialize(const std::vector<Model*>& models) override;

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
    /// 衝突時の応答
    /// </summary>
    /// <param name="other"></param>
    void OnCollision([[maybe_unused]] Collider* other) override;

    /// <summary>
    /// 中心座標取得
    /// </summary>
    Vector3 GetCenterPos() const override;

    /// <summary>
    /// 対象者をセット
    /// </summary>
    void SetTarget(Player* pos);

    /// <summary>
    /// ターゲットに向けて移動
    /// </summary>
    void MoveForTarget();

    void MoveForStronghold();

    /// <summary>
    /// 攻撃
    /// </summary>
    void Attack();

    /// <summary>
    /// プレイヤー側拠点のポインタをセット
    /// </summary>
    void SetPlayerStronghold(PlayerStronghold* stronghold);

private:
    Player* target_ = nullptr;  // 追従対象
    PlayerStronghold* strongholdPtr = nullptr;
    float speed_ = 0.3f;  // 移動速度
    int coolTime_ = 150;  // 3秒に一回攻撃
    bool isAttack_ = false;
    Vector3 velocity_ {};

    std::unique_ptr<Collider> armCollider_;  // 腕用コライダー
};
