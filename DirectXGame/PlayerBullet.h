#pragma once
#include "Bullet.h"

class PlayerBullet 
	: public Bullet {

private:
	Vector3 velocity_;

public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	/// <param name="position"></param>
	void Initialize ( Model* model, const Vector3& position,const Vector3& velocity );
	
	/// <summary>
	/// 更新
	/// </summary>
	void Update ()override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw (const ViewProjection& viewProjection);

	///========================
	/// アクセッサー
	/// =======================
	Vector3 GetVelocity()const{ return velocity_; }

};
