#pragma once
#include"Enemy.h"

class EnemyStronghold;
class Player;

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

	/// <summary>
	/// リザルトシーン用の更新
	/// </summary>
	void UpdateResultScene();

	void OnCollision([[maybe_unused]] Collider* other)override;

	/// <summary>
	/// 中心座標取得
	/// </summary>
	Vector3 GetCenterPos()const override;

	void SetStronghold(EnemyStronghold* stronghold);

	void SetPlayer(Player* player);

private:
	EnemyStronghold* stronghold_ = nullptr;
	Player* player_ = nullptr;

	float timer_ = 0.0f; // タイマー
	const float chargeInterval_ = 190.0f; // 突進の間隔時間
};