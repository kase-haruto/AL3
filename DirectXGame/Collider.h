#pragma once
#include"Vector3.h"
#include"WorldTransform.h"

class Collider{
public:
	virtual ~Collider() = default;

	/// <summary>
	/// ワールドトランスフォーむの初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 衝突時の処理
	/// </summary>
	virtual void OnCollision(){}

	virtual Vector3 GetCenterPos()const = 0;
	float GetRadius()const{ return radius_; }
	void SetRadius(float radius){ radius_ = radius; }

private:
	float radius_ = 1.0f;
	//ワールドトランスフォーム
	WorldTransform worldTransform_;
};

