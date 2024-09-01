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
	void Initialize(const Vector3& pos,const Vector2& scale,const std::string& filePath = "./Resources/cube/cube.jpg");

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 選択されているか
	/// </summary>
	/// <param name="isSelect"></param>
	void SetIsSelected(bool isSelect);

	Vector2 GetPos()const;

	void SetPos(const Vector2& pos);

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

