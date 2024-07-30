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

private:
	//コライダー
	std::list<Collider*> colliders_;
	//デバッグ表示用モデル
	std::unique_ptr<Model> debugModel_;
};

