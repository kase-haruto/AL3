#pragma once
#include "Actor.h"
#include"Input.h"
#include"PlayerBullet.h"
#include<list>
#include<memory>

class Player : 
	public Actor {
private:
	Vector3 velocity_;
	//弾
	//std::list<PlayerBullet*> bullets_;
	std::list<std::unique_ptr<PlayerBullet>> bullets_;
	Input* input_ = nullptr;
	
	

private://メンバ関数
	/// <summary>
	/// playerの移動処理を行います
	/// </summary>
	void Move();
	/// <summary>
	/// playerの回転を行います
	/// </summary>
	void Rotate();
	/// <summary>
	/// 弾を撃つ関数
	/// </summary>
	void Shoot ();

public://メンバ関数

	Player();
	~Player ()override;

	void Init(Model* model);
	void Update() override;
	void Draw(ViewProjection& viewprojection) override;

	void OnCollision()override;

	Vector3 GetVelocity() const { return velocity_; }
	void SetVelocity(const Vector3 velocity) { velocity_ = velocity; }
	Vector3 GetWorldPosition()const override;

	/// <summary>
	/// 弾のリストを取得
	/// </summary>
	/// <returns></returns>
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets()const{ return bullets_; }

};
