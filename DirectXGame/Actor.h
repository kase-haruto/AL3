#pragma once
#include"WorldTransform.h"
#include"ViewProjection.h"
#include"Model.h"

#include<vector>
#include<memory>
#include<stdint.h>

class Actor{
protected:
	uint32_t textureHandle_;
	uint32_t color_;

	//===================================
	//	3d用
	WorldTransform worldTransform_;
	std::vector<Model*>models_;
	bool isUseTexture = false;

public:
	Actor(){}
	virtual ~Actor() = default;

	virtual void Initialize(const std::vector<Model*>& models);
	virtual void Update();
	virtual void Draw(const ViewProjection& viewProjection);

	///================================
	///	アクセッサ
	///================================
	const WorldTransform& GetWorldTransform();
	Vector3 GetWorldPosition()const;

	Vector3 GetTranslation()const;
	void SetPos(const Vector3& pos);

	Vector3 GetRotation()const;
	void SetRotation(const Vector3& rotation);

	uint32_t GetColor()const;
	void SetColor(const uint32_t& color);
};

