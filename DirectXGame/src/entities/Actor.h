#pragma once
#include"ViewProjection.h"
#include"Model.h"
#include"Collider.h"

#include<vector>
#include<memory>
#include<stdint.h>

class Actor :public Collider{


public:
	Actor() = default;
	virtual ~Actor() = default;

	virtual void Initialize(const std::vector<Model*>& models);
	virtual void Update();
	virtual void Draw(const ViewProjection& viewProjection);
	virtual Vector3 GetCenterPos()const override;

	void UpdateMatrix();

	/// <summary>
	/// 衝突時の処理
	/// </summary>
	virtual void OnCollision([[maybe_unused]] Collider* other)override;

	///================================
	///	アクセッサ
	///================================
	const WorldTransform& GetWorldTransform();
	Vector3 GetWorldPosition()const;
	void SetTranslation(const Vector3& translation);
	void SetTranslationX(const float translation);
	void SetTranslationY(const float translation);
	void SetTranslationZ(const float translation);
	void SetRotation(const Vector3& rotation);
	void SetRotationX(const float rotatino);
	void SetRotationY(const float rotation);
	void SetRotationZ(const float rotation);

	Vector3 GetTranslation()const;
	void SetPos(const Vector3& pos);

	Vector3 GetRotation()const;

	int GetLife()const{ return life_; }
	void SetLife(int life){ life_ = life; }

	uint32_t GetColor()const;
	void SetColor(const uint32_t& color);

	bool GetIsAlive()const{ return isAlive_; }

protected:
	uint32_t textureHandle_;
	uint32_t color_;
	int life_ = 5;
	bool isAlive_ = true;

	//===================================
	//	3d用
	WorldTransform worldTransform_;
	Vector3 direction_;
	std::vector<Model*>models_;
	bool isUseTexture = false;
};

