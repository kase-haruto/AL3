#pragma once
#include "Player.h"


class PlayerStronghold;

class ProtectPlayer :
    public Player{
public:
    ProtectPlayer() = default;
    ~ProtectPlayer() override = default;

    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name="model">プレイヤーのモデル配列</param>
    void Initialize(const std::vector<Model*>& model) override;

    /// <summary>
    /// uiの初期化
    /// </summary>
    void InitializeUi()override;

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
    /// 衝突時の反応
    /// </summary>
    void OnCollision([[maybe_unused]] Collider* other) override;

    void SetStrongholdPtr(PlayerStronghold* stronghold);

private:
    PlayerStronghold* strongholdPtr_ = nullptr;

    //プレイヤー移動制限用モデル
    std::unique_ptr<Model>moveRangeModel_ = nullptr;

    //モデル用transform
    WorldTransform rangeTransform_;


};

