#pragma once
class Collider{
public:
	virtual ~Collider() = default;

	/// <summary>
	/// 衝突時の処理
	/// </summary>
	virtual void OnCollision(){}

	virtual Vector3 GetCenterPos()const = 0;
	float GetRadius()const{ return radius_; }
	void SetRadius(float radius){ radius_ = radius; }
private:
	float radius_;
};

