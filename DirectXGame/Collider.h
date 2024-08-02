#pragma once
#include"Vector3.h"
#include"WorldTransform.h"
#include"ViewProjection.h"
#include"Model.h"

#include <stdint.h>

/// <summary>
/// オブジェクト基底クラス
/// </summary>
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

	virtual Vector3 GetCenterPos()const = 0;
	float GetRadius()const{ return radius_; }

	const Vector3& GetRotation()const;
	const Vector3& GetTranslation()const;
	const Vector3& GetScale()const;

	const WorldTransform& GetWorldTransform()const{ return worldTransform_; }

	//識別IDの取得
	uint32_t GetTypeId()const{ return typeID_; }

	void SetRotation(const Vector3& rotation);
	void SetRotationX(const float rotation);
	void SetRotationY(const float rotation);
	void SetRotationZ(const float rotation);

	void SetTranslation(const Vector3& translation);
	void SetTranslationX(const float translation);
	void SetTranslationY(const float translation);
	void SetTranslationZ(const float translation);

	void SetScale(const Vector3& scale);

	void SetRadius(float radius){ radius_ = radius; }

	void SetTypeID(uint32_t typeID){ typeID_ = typeID; }
protected:
	float radius_ = 1.5f;
	//ワールドトランスフォーム
	WorldTransform worldTransform_;

	//オブジェクト識別ID
	uint32_t typeID_ = 0u;
};

