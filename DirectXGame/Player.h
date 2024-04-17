#pragma once
#include "Actor.h"
class Player : 
	public Actor {
private:
	Vector3 velocity_;

private://メンバ関数
	void Move();

public://メンバ関数

	Player();
	~Player()override;

	void Init(Vector3 velocity);
	void Update() override;
	void Draw(ViewProjection& viewprojection) override;

	Vector3 GetVelocity() const { return velocity_; }
	void SetVelocity(const Vector3 velocity) { velocity_ = velocity; }
};
