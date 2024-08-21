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

	///================================
	///	アクセッサ
	///================================
	const WorldTransform& GetWorldTransform();
	Vector3 GetWorldPosition()const;
	void SetTranslation(const Vector3& translation);
	void SetRotation(const Vector3& rotation);
	void SetRotationX(const float rotatino);
	void SetRotationY(const float rotation);
	void SetRotationZ(const float rotation);

	Vector3 GetTranslation()const;
	void SetPos(const Vector3& pos);

	Vector3 GetRotation()const;


	uint32_t GetColor()const;
	void SetColor(const uint32_t& color);

protected:
	uint32_t textureHandle_;
	uint32_t color_;

	//===================================
	//	3d用
	WorldTransform worldTransform_;
	Vector3 direction_;
	std::vector<Model*>models_;
	bool isUseTexture = false;
};

