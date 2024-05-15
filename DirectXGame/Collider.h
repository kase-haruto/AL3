#pragma once
#include"Vector3.h"

/// <summary>
/// 衝突判定オブジェクト
/// </summary>
class Collider{
private:
	float radius_ = 1.0f;

public:
	Collider(){};
	virtual ~Collider(){};

	float GetRadius()const{ return radius_; };
	void SetRadius(const float radius){ radius_ = radius; }

	virtual void OnCollision()=0;
	virtual Vector3 GetWorldPosition()const = 0;
};