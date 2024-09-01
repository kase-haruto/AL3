#pragma once
#include"EnemyManager.h"
#include"Model.h"
#include"WorldTransform.h"
#include"ViewProjection.h"
#include"PlayerStronghold.h"
#include"EnemyManager.h"


#include<memory>

/// <summary>
/// 敵がわいてくるオブジェクト
/// </summary>
class EnemyStation{
public:
	/// <summary>
	/// コンストラクタ/
	/// デストラクタ
	/// </summary>
	EnemyStation() = default;
	~EnemyStation() = default;

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
	/// <param name="viewProjection"></param>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// 座標をセット
	/// </summary>
	/// <param name="pos"></param>
	void SetPosition(const Vector3& pos);

	/// <summary>
	/// プレイヤー側の拠点のポインタ取得
	/// </summary>
	/// <param name="stronghold"></param>
	void SetPlayerStrongholdPtr(PlayerStronghold* stronghold);

	/// <summary>
	/// 敵管理クラスのポインタのセット
	/// </summary>
	/// <param name="enemyManager"></param>
	void SetEnemyManager(EnemyManager* enemyManager);

	/// <summary>
	/// 敵のスポーン
	/// </summary>
	void SpawnEnemy();

private:
	//transform
	WorldTransform worldTransform_;

	//描画用モデル
	std::unique_ptr<Model> modelStation_ = nullptr;

	//敵のスポーン
	int spawnTimer_;

	const int kLimitTime_ = 480;//7秒ごとにスポーン

	//プレイヤー側の拠点のポインタ
	PlayerStronghold* playerStrongholdPtr_ = nullptr;

	//敵管理クラス
	EnemyManager* enemyManager_ = nullptr;
};