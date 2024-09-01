#pragma once

#include"Model.h"
#include"ViewProjection.h"
#include"WorldTransform.h"

enum SelectMode{
	PlayGame,
	Setting,
	ExitGame,
};

class TransitionSelectCube{
public:
	TransitionSelectCube() = default;
	~TransitionSelectCube() = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize(const Vector3& pos);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

private:
	/// <summary>
	/// 回転
	/// </summary>
	void TransitionMode();

private:
	//モデル
	std::unique_ptr<Model> modelCube_;

	//transform
	WorldTransform worldTransform_;

	//回転角度
	const float rotateAngle_ = 1.55f;

	//現在の選択モード:初期はゲームプレイ
	uint16_t currentMode_ = PlayGame;
};

