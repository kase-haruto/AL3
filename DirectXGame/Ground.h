#pragma once
#include"Vector3.h"
#include"ViewProjection.h"
#include"Model.h"
#include"WorldTransform.h"

/// <summary>
/// 地面
/// </summary>
class Ground{
private:
	Model* model_;
	WorldTransform worldTransform_;

public:
	Ground(Model* model);
	~Ground();
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="scale"></param>
	void Initialize(const Vector3& scale);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection* viewProjection);
};

