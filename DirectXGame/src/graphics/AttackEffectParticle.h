#pragma once
#include"Model.h"
#include"ViewProjection.h"
#include"WorldTransform.h"

#include <chrono>
#include<memory>

/// <summary>
/// 攻撃を当てたときのエフェクト
/// </summary>
class AttackEffectParticle{
public:

	AttackEffectParticle();
	~AttackEffectParticle()= default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	void Initialize(const WorldTransform* target);
	/// <summary>
	/// エフェクト開始
	/// </summary>
	void PlayEffect();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// エフェクト対象
	/// </summary>
	/// <param name="target"></param>
	void SetTarget(const WorldTransform* target);
	
	/// <summary>
	/// effectが終了したか確認するメソッド
	/// </summary>
	/// <returns></returns>
	bool IsEffectFinished()const;

private:

	
	std::unique_ptr<Model> model_ = nullptr;

	const WorldTransform* target_ = nullptr;
	WorldTransform worldTransform_;

	float movingStep_;
	float effectDuration_; // エフェクトの持続時間
	std::chrono::time_point<std::chrono::steady_clock> startTime_; // エフェクト開始時間

};

