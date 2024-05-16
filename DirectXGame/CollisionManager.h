#pragma once

#include"Collider.h"

#include<list>
#include<memory>

class CollisionManager{
private:

	std::list<Collider*>colliders_; //オブジェクトリスト

public:
	CollisionManager();
	~CollisionManager();
	CollisionManager(const CollisionManager&) = delete;
	CollisionManager& operator= (const CollisionManager&) = delete;

	/// <summary>
	/// 当たり判定の更新
	/// </summary>
	void Update();
	/// <summary>
	/// リストから削除
	/// </summary>
	/// <param name="collider"></param>
	void RemoveCollider(Collider* collider);
	/// <summary>
	/// コライダーリストに追加
	/// </summary>
	/// <param name="collider"></param>
	void SetCollider(Collider* collider);
private:
	/// <summary>
	/// 総当たりの判定
	/// </summary>
	void CheckAllCollisions();
	/// <summary>
	/// 二つのコライダーの判定と応答
	/// </summary>
	/// <param name="colliderA"></param>
	/// <param name="colliderB"></param>
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);
	/// <summary>
	/// 距離を測る
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <returns></returns>
	float CheckDistance(Vector3 v1, Vector3 v2);
	/// <summary>
	/// 衝突してるか判定
	/// </summary>
	/// <param name="distance"></param>
	/// <param name="radius1"></param>
	/// <param name="radius2"></param>
	/// <returns></returns>
	bool IsOnCollision(float distance, float radius1, float radius2);
	


public:

	/// <summary>
	/// インスタンスを取得
	/// </summary>
	/// <returns></returns>
	static CollisionManager* GetInstance();
};

