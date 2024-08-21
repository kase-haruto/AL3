#pragma once
#include"WorldTransform.h"

/// <summary>
/// ボタン
/// </summary>
class Button{
public:
	Button() = default;
	~Button() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const Vector3& pos,const Vector3& scale);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
private:
	//transform
	WorldTransform worldTransform_;

	//選ばれているか
	bool isSelected_;
};

