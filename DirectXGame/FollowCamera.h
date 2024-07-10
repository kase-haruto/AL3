#pragma once
#include"ViewProjection.h"
#include"WorldTransform.h"

/// <summary>
/// 追従カメラ
/// </summary>
class FollowCamera {
public:
	FollowCamera();
	~FollowCamera();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 追従対象をセット
	/// </summary>
	/// <param name="target"></param>
	void SetTarget(const WorldTransform* target);
	/// <summary>
	/// viewProjectionの取得
	/// </summary>
	/// <returns></returns>
	const ViewProjection& GetViewProjection();
	/// <summary>
	/// offsetの計算
	/// </summary>
	/// <returns></returns>
	Vector3 CalculateOffset();
private:
	/// <summary>
	/// 旋回
	/// </summary>
	void Turning();
	/// <summary>
	/// 追従
	/// </summary>
	void Adulation();
	/// <summary>
	/// reset
	/// </summary>
	void Reset();

private:
	//ビュープロジェクション
	ViewProjection viewProjection_;
	//追従対象
	const WorldTransform* target_ = nullptr;
	//追従対象の残像座標
	Vector3 interTarget_ = {};
	//目標角度
	float destinationAngleY_ = 0.0f;
};

