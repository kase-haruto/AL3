#pragma once
#include"Vector3.h"
#include"WorldTransform.h"
#include"ViewProjection.h"
#include"Model.h"
#include"CollisionTypeIdDef.h"

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
	virtual void OnCollision([[maybe_unused]] Collider* other){}

	///=================================================================
	///		アクセッサ
	///=================================================================

	virtual Vector3 GetCenterPos()const{
		const Vector3 offset = {0.0f,1.0f,0.0f};

		Vector3 worldPos = Matrix4x4::Transform(offset, worldTransform_.matWorld_);
		return worldPos;
	};

	void SetTranslation(const Vector3& translation){ worldTransform_.translation_ = translation; }


	float GetRadius()const{ return radius_; }
	//識別IDの取得
	uint32_t GetTypeID()const{ return typeID_; }

	void SetRadius(float radius){ radius_ = radius; }
	void SetTypeID(uint32_t typeID){ typeID_ = typeID; }

private:
	float radius_ = 1.5f;
	//ワールドトランスフォーム
	WorldTransform worldTransform_;

	uint32_t typeID_ = 0u;
};

