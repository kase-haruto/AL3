#pragma once
#include "Collider.h"
#include "Model.h"
#include "Actor.h"
#include "BaseStronghold.h"
#include "Player.h"
#include "EnemyStronghold.h"
#include"PlayerStronghold.h"
#include <list>
#include <memory>

/// <summary>
/// 衝突マネージャー
/// </summary>
class CollisionManager{
public:
    /// <summary>
    /// インスタンスを取得
    /// </summary>
    static CollisionManager* GetInstance(){
        static CollisionManager instance;
        return &instance;
    }

    ~CollisionManager() = default;

    // コピーコンストラクタと代入演算子を削除
    CollisionManager(const CollisionManager&) = delete;
    CollisionManager& operator=(const CollisionManager&) = delete;

    /// <summary>
    /// 初期化
    /// </summary>
    void Initialize();

    /// <summary>
    /// コライダーのリセット
    /// </summary>
    void Reset();

    /// <summary>
    /// 二つのコライダーの衝突時の応答
    /// </summary>
    void CheckCollisionPair(Collider* colliderA, Collider* colliderB);

    /// <summary>
    /// 総当たりの衝突判定
    /// </summary>
    void CheckAllCollidion();

    /// <summary>
    /// コライダーの追加
    /// </summary>
    void AddCollider(Collider* collider);

    /// <summary>
    /// コライダーの削除
    /// </summary>
    /// <param name="collider"></param>
    void RemoveCollider(Collider* collider);

    /// <summary>
    /// トランスフォームの更新
    /// </summary>
    void UpdateWorldTransform();

    /// <summary>
    /// 判定の可視化
    /// </summary>
    void Draw(const ViewProjection& viewProjection);

    /// <summary>
    /// 調整項目の適用
    /// </summary>
    void ApplyGlobalVariables();

    /// <summary>
    /// プレイヤーと敵の拠点の判定
    /// </summary>
    void Character2Stronghold(Actor* character, BaseStronghold* stronghold);

    void Player2Stronghold(Player* player, EnemyStronghold* enemyStronghold);

    void Enemy2Stronghold(Enemy* enemy, PlayerStronghold* stronghold);

private:
    CollisionManager();  // コンストラクタをプライベートにする

    // コライダー
    std::list<Collider*> colliders_;

    // デバッグ表示用モデル
    std::unique_ptr<Model> debugModel_;

    bool isDrawCollider_ = false;
};
