#pragma once
#include "Model.h"
#include "Vector3.h"
#include "WorldTransform.h"
#include"TextureManager.h"
#include <stdint.h>
#include"Collider.h"

class Actor 
	:public Collider{
protected: // メンバ変数
	uint32_t color_;
	uint32_t textuerHandle_;
	float radius_;
	bool isAlive_ = true;

	//======================
	//	3d関連
	Model* model_ = nullptr;
	WorldTransform worldTransform_;

public: // メンバ関数
	virtual ~Actor() {}


	void Init();
	void Update();
	virtual void Draw(ViewProjection& viewprojection);


	//=====================
	//	アクセッサ
	//=====================

	Vector3 GetTranslate() const { return worldTransform_.translation_; }
	void SetTranslate(const Vector3 translate) { worldTransform_.translation_ = translate; }

	Vector3 GetRotate() const { return worldTransform_.rotation_; }
	void SetRotate(const Vector3 rotate) { worldTransform_.rotation_ = rotate; }

	Vector3 GetScale() const { return worldTransform_.scale_; }
	void SetScale(const Vector3 scale) { worldTransform_.scale_ = scale; }

	uint32_t GetColor() const { return color_; }
	void SetColor(const uint32_t color) { color_ = color; }

	float GetRadius()const{ return radius_; }

	bool GetIsAlive()const{ return isAlive_; }
};
