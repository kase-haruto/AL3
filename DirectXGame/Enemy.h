#pragma once
#include "Actor.h"
#include"EnemyBullet.h"
#include<memory>
#include"BaseEnemyState.h"
#include"TimedCall.h"

//行動フェーズ
enum class Phase : uint32_t{
	Approach,//接近する
	Leave,//離脱する
};

class Player;

class Enemy :
	public Actor{
	Vector3 velocity_;
	bool isMove = false;
	const uint32_t kShootInterval = 60;

	std::list<std::unique_ptr<EnemyBullet>> bullets_;
	std::unique_ptr<BaseEnemyState> state_;
	//次元発動のリスト
	std::list<std::unique_ptr<TimedCall>> timedCalls_;
	Phase currentPhase;

	//自キャラ
	Player* player_ = nullptr;
private://メンバ関数
	
	/// <summary>
	/// 弾を撃つ
	/// </summary>
	void Shoot();

public:

	Enemy();
	~Enemy()override;

	void Init(Model* model);
	void Update()override;
	void Draw(ViewProjection& viewprojection)override;

	void OnCollision()override;

	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// 発射とタイマーのリセット
	/// </summary>
	void ShootAndLisetTimer();

	/// <summary>
	/// 状態遷移
	/// </summary>
	void TransitionState(std::unique_ptr<BaseEnemyState>state);

	/// <summary>
	/// 接近フェーズの初期化
	/// </summary>
	void ApproachInitialize();

	void Approach2Leave();

	void SetIsMove(const bool isMove_){ isMove = isMove_; }

	Vector3 GetVelocity()const{ return velocity_; }
	void SetVelocity(const Vector3& velocity){ velocity_ = velocity; }

	void SetPhase(const Phase& phase){ currentPhase = phase; }

	uint32_t GetShootInterval()const{ return kShootInterval; }

	void SetPlayer(Player* player){ player_ = player; }
	 
	Vector3 GetWorldPosition()const override;

	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets()const {return bullets_; }
};

