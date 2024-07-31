#pragma once
#include"Vector3.h"
#include"WorldTransform.h"
#include"ViewProjection.h"
#include"Model.h"

class Collider{
public:
	virtual ~Collider() = default;

	/// <summary>
	/// ワールドトランスフォーむの初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// トランスフォームの更新
	/// </summary>
	void UpdateTransform();
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="modl"></param>
	/// <param name="viewProjection"></param>
	void Draw(Model* modl, const ViewProjection& viewProjection);

	/// <summary>
	/// 衝突時の処理
	/// </summary>
	virtual void OnCollision(){}

	///=================================================================
	///		アクセッサ
	///=================================================================

	virtual Vector3 GetCenterPos()const = 0;
	float GetRadius()const{ return radius_; }
	void SetRadius(float radius){ radius_ = radius; }

private:
	float radius_ = 1.5f;
	//ワールドトランスフォーム
	WorldTransform worldTransform_;
};

