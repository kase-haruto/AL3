#pragma once
#include"WorldTransform.h"
#include"Sprite.h"

#include<cstdint>
#include<string>
#include<memory>

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
	void Initialize(const Vector3& pos,const Vector3& scale,const std::string& filePath = "./Resources/cube/cube.jpg");

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

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;

	//スプライト
	std::unique_ptr<Sprite> sprite_ = nullptr;

	//選ばれているか
	bool isSelected_;
};

