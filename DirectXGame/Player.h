#pragma once
#include"Actor.h"
#include"MyFunc.h"
#include"Model.h"

#include<vector>


class Player :public Actor{

	/// <summary>
	/// プレイヤーのパーツ
	/// </summary>
	enum class Parts{
		body,
		head,
		L_arm,
		R_arm,
		weapon,
		partsCount
	};

public:
	Player();
	~Player()override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const std::vector<Model*>& model)override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection)override;
	/// <summary>
	/// 追尾カメラ
	/// </summary>
	void SetViewProjection(const ViewProjection* viewProjection);

private:
	/// <summary>
	/// 移動
	/// </summary>
	void Move();
	/// <summary>
	/// 浮遊行動の初期化
	/// </summary>
	void InitializeFloatingAction();
	/// <summary>
	/// 浮遊行動の更新
	/// </summary>
	void UpdateFloatingAction();
	/// <summary>
	/// 通常行動更新
	/// </summary>
	void BehaviorRootUpdate();
	/// <summary>
	/// 攻撃行動更新
	/// </summary>
	void BehaviorAttackUpdate();
private:
	//カメラのビュープロジェクション
	const ViewProjection* viewPorjection_ = nullptr;
	float targetAngle;

	//パーツ事の変数
	std::vector<std::unique_ptr< WorldTransform>> partsTransform_;

	//浮遊ギミックの媒介変数
	float floatingParameter_ = 0.0f;

	bool isAttack_ = false;
};

