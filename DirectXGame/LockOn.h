#pragma once
#include"Sprite.h"
#include"Enemy.h"

#include<numbers>
#include<memory>

/// <summary>
/// ロックオン
/// </summary>
class LockOn{
public:
	LockOn();
	~LockOn(){}

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update(const std::list<std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewProjection);
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:
	/// <summary>
	/// 適用
	/// </summary>
	void ApplyGlobalVariables();
	/// <summary>
	/// ロックオン対象を決定
	/// </summary>
	void SelectTarget(const std::list<std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewProjection);
private:
	std::unique_ptr<Sprite, void(*)(Sprite*)> lockOnMark_;
	Vector2 markerPos_ {640.0f,360.0f};
	//ロックオン対象
	const Enemy* target_ = nullptr;

	//最小距離
	float minDistance_ = 10.0f;
	//最大距離
	float maxDistanec_ = 30.0f;
	// 角度をラジアンに変換する定数
	const float kDegreeToRadian = float(std::numbers::pi) / 180.0f;

	// 角度範囲（度からラジアンに変換）
	float angleRange_ = 20.0f * kDegreeToRadian;
};

