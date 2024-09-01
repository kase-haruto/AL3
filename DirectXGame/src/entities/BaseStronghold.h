#pragma once

#include"Model.h"	
#include"ViewProjection.h"
#include"WorldTransform.h"

#include<memory>
#include<cstdint>

/// <summary>
/// 拠点の基底クラス
/// </summary>
class BaseStronghold{
public:
	/// <summary>
	/// コンストラクタ
	/// デストラクタ
	/// </summary>
	BaseStronghold() = default;
	virtual ~BaseStronghold() = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="model"></param>
	virtual void Initialize(Model* model);

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update(){};

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection"></param>
	virtual void Draw(const ViewProjection& viewProjection);

	void ReduceLife();

	void UpdateMatrix();

	//======================================================
	//	アクセッサ
	//======================================================
	uint32_t GetLife()const;
	const WorldTransform& GetWorldTransform()const;
	Vector3 GetCenterPos()const;
	float GetRadius()const;
	Vector3 GetWorldPosition()const;
	bool GetIsTaked()const;

	void SetLife(const uint32_t life);
	void SetTranslation(const Vector3& translate);
	void SetRotate(const Vector3& rotate);
	void SetScale(const Vector3& scale);
	void SetIsTaked(bool isTaked);
	void SetRangeModel(Model* model);

protected:
	//使用するモデル
	Model* model_;

	//範囲の描画用モデル
	Model* rangeModel_;

	//拠点の体力
	uint32_t life_;

	//拠点のtransform
	WorldTransform worldTransform_;

	//拠点の範囲の半径
	float rangeRadius_;

	//拠点がとられたかどうか
	bool isTaked_ = false;

	//ゲットカウント
	uint32_t holdTimer_ = 0;

};

