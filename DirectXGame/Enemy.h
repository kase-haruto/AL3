#pragma once
#include "Actor.h"
#include"EnemyBullet.h"
#include<memory>
#include"BaseEnemyState.h"


//行動フェーズ
enum class Phase{
	Approach,//接近する
	Leave,//離脱する
};


class Enemy :
	public Actor{
	Vector3 velocity_;
	bool isMove = false;

	uint32_t cooltime_ = 0;
	const uint32_t kShootInterval = 60;

	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	std::unique_ptr<BaseEnemyState> state_;


	//フェーズ
	Phase phase_ = Phase::Approach;

private://メンバ関数

	/// <summary>
	/// メンバ関数ポインタ
	/// </summary>
	static void (Enemy::* spFuncTable[])();

public:

	Enemy();
	~Enemy()override;

	void Init(Model* model);
	void Update()override;
	void Draw(ViewProjection& viewprojection)override;

	/// <summary>
	/// 弾を撃つ
	/// </summary>
	void Shoot();

	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// 状態遷移
	/// </summary>
	void TransitionState(std::unique_ptr<BaseEnemyState>state);

	/// <summary>
	/// 接近フェーズの初期化
	/// </summary>
	void ApproachInitialize();

	void SetIsMove(const bool isMove_){ isMove = isMove_; }

	Vector3 GetVelocity()const{ return velocity_; }
	void SetVelocity(const Vector3& velocity){ velocity_ = velocity; }

	void SetPhase(const Phase& phase){ phase_ = phase; }

	uint32_t GetCoolTime()const{ return cooltime_; }
	void SetCoolTime(const uint32_t cooltime){ cooltime_ = cooltime; }

	uint32_t GetShootInterval()const{ return kShootInterval; }
};

