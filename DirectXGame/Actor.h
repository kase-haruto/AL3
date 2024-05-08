#pragma once
#include "Model.h"
#include "Vector3.h"
#include "WorldTransform.h"
#include"TextureManager.h"
#include <stdint.h>

class Actor {
protected: // メンバ変数
	uint32_t color_;
	uint32_t textuerHandle_;


	//======================
	//	3d関連
	Model* model_ = nullptr;
	WorldTransform worldTransform_;

public: // メンバ関数
	virtual ~Actor() {}


	void Init();
	virtual void Update() = 0;
	virtual void Draw(ViewProjection& viewprojection);

	//衝突した際の関数
	virtual void OnCollision() = 0;

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
};
