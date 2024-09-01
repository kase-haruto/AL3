#pragma once
#include"Enemy.h"


class ProtectEnemy :
	public Enemy{

public:
	/// <summary>
	/// コンストラクタ/デストラクタ
	/// </summary>
	ProtectEnemy() = default;
	~ProtectEnemy()override = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="models"></param>
	void Initialize(const std::vector<Model*>& models)override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(const ViewProjection& viewProjection)override;

	void OnCollision([[maybe_unused]] Collider* other)override;

	/// <summary>
	/// 中心座標取得
	/// </summary>
	Vector3 GetCenterPos()const override;

private:

};