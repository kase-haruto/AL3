#pragma once
#include"Collider.h"
#include"Model.h"

#include<list>

/// <summary>
/// 衝突マネージャー
/// </summary>
class CollisionManager{
public:
	CollisionManager();
	~CollisionManager() = default;

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
	/// <param name="colliderA"></param>
	/// <param name="colliderB"></param>
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);

	/// <summary>
	/// 総当たりの衝突判定
	/// </summary>
	void CheckAllCollidion();

	/// <summary>
	/// コライダーの追加
	/// </summary>
	/// <param name="collider"></param>
	void AddCollider(Collider* collider);

	/// <summary>
	/// トランスフォームの更新
	/// </summary>
	void UpdateWorldTransform();

	/// <summary>
	/// 判定の可視化
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// 調整項目の適用
	/// </summary>
	void ApplyGlobalVariables();

private:
	//コライダー
	std::list<Collider*> colliders_;
	//デバッグ表示用モデル
	std::unique_ptr<Model> debugModel_;

	bool isDrawCollider_ = false;
};

