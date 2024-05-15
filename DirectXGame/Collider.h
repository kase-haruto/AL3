#pragma once
#include"Vector3.h"
#include<stdint.h>

/// <summary>
/// 衝突判定オブジェクト
/// </summary>
class Collider{
private:
	float radius_ = 1.0f;

	//衝突属性
	uint32_t collisionAttribute_ = 0xffffffff;
	//衝突マスク(相手)
	uint32_t collisionMask_ = 0xffffffff;

public:
	Collider(){};
	virtual ~Collider(){};

	float GetRadius()const{ return radius_; };
	void SetRadius(const float radius){ radius_ = radius; }

	uint32_t GetCollisionAttribute()const{ return collisionAttribute_; }
	void SetCollisionAttribute(const uint32_t attribute){ collisionAttribute_ = attribute; }

	uint32_t GetCollisionMask()const{ return collisionMask_; }
	void SetCollisionMask(const uint32_t mask){ collisionMask_ = mask; }


	virtual void OnCollision()=0;
	virtual Vector3 GetWorldPosition()const = 0;
	

};