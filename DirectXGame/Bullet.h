#pragma once
#include "Model.h"
#include "Vector3.h"
#include"WorldTransform.h"
#include"Model.h"
#include"TextureManager.h"
#include"Collider.h"

/// <summary>
/// 弾の基底クラス
/// </summary>
class Bullet
:public Collider{
protected:
	WorldTransform worldTransform_;
	Model* model_;
	uint32_t textuerHandle_;

	float radius_;
public: // メンバ関数
	Bullet(){ };
	virtual ~Bullet(){};

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	/// <param name="position"></param>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw(const ViewProjection& viewProjection);


	float GetRadius()const{ return radius_; }

	Vector3 GetWorldPosition()const{
		Vector3 wPos;
		wPos = worldTransform_.translation_;
		return wPos;
	}
};
