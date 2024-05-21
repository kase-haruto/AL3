#pragma once
#include"WorldTransform.h"
#include"Model.h"

/// <summary>
/// 天球
/// </summary>
class Skydome{
private:

	//ワールド変換データ
WorldTransform wTransform_;
	//モデル
	Model* model_ = nullptr;

public:
	Skydome(Model* model);
	~Skydome();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);
};

