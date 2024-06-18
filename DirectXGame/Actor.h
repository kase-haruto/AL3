#pragma once
#include"WorldTransform.h"
#include"Model.h"
#include"ViewProjection.h"

#include<memory>
#include<stdint.h>

class Actor{
protected:
	uint32_t textureHandle_;
	uint32_t color_;
	
	//===================================
	//	3d用
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	
	ViewProjection* viewProjection_;
	bool isUseTexture = false;

public:
	Actor(){}
	virtual ~Actor() = default;

	void Initialize(){};
	void Update(){};
	virtual void Draw();

	///================================
	///	アクセッサ
	///================================
	Vector3 GetWorldPosition()const;

	Vector3 GetTranslation()const;
	void SetTranslatiion(const Vector3& translation);

	Vector3 GetRotation()const;
	void SetRotation(const Vector3& rotation);

	uint32_t GetColor()const;
	void SetColor(const uint32_t& color);
};

