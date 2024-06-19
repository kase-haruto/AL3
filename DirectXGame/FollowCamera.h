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
	void SetTarget(const WorldTransform* target){ target_ = target; }
	/// <summary>
	/// viewProjectionの取得
	/// </summary>
	/// <returns></returns>
	const ViewProjection& GetViewProjection();

private:
	/// <summary>
	/// 旋回
	/// </summary>
	void Turning();
	/// <summary>
	/// 追従
	/// </summary>
	void Adulation();

private:
	//ビュープロジェクション
	ViewProjection viewProjection_;
	//追従対象
	const WorldTransform* target_ = nullptr;

};

