#pragma once
#include"Actor.h"

class Enemy:public Actor{
	/// <summary>
	/// 敵のパーツ
	/// </summary>
	enum class Parts{
		body,
		arm,
		partsCount
	};

public:
	/// <summary>
	/// コンストラクタ/デストラクタ
	/// </summary>
	Enemy();
	~Enemy();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="models"></param>
	void Initialize(const std::vector<Model*>&models)override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(const ViewProjection& viewProjection)override;

	void OnCollision([[maybe_unused]] Collider* other)override;

	/// <summary>
	/// 中心座標取得
	/// </summary>
	Vector3 GetCenterPos()const override;

private:
	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();
	/// <summary>
	/// 腕の動き
	/// </summary>
	void ArmWave();

private:
	//パーツ事の変数
	std::vector<std::unique_ptr< WorldTransform>> partsTransform_;
	float waveParameter_ = 0;
};

