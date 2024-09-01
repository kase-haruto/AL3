#pragma once
#include"BaseStronghold.h"

#include"ProtectEnemy.h"
#include"AttackPlayer.h"

class EnemyManager;
class Player;

class EnemyStronghold final
	:public BaseStronghold{
public:
	/// <summary>
	/// コンストラクタ
	/// デストラクタ
	/// </summary>
	EnemyStronghold() = default;
	~EnemyStronghold()override = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="model"></param>
	void Initialize(Model* model)override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update()override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(const ViewProjection& viewProjection)override;

	/// <summary>
	/// 敵管理クラスのポインタをセット
	/// </summary>
	/// <param name="manager"></param>
	void SetEnemyManager(const std::unique_ptr<EnemyManager>& manager);

	/// <summary>
	/// プレイヤーのポインタ
	/// </summary>
	/// <param name="player"></param>
	void SetPlayerPtr(const std::unique_ptr<AttackPlayer>& player);

	/// <summary>
	/// 拠点防衛敵
	/// </summary>
	/// <param name="enemy"></param>
	void SetProtectEnemy(ProtectEnemy* enemy);


	/// <summary>
	/// 守るキャラが消えたときの処理
	/// </summary>
	void ExtinctionCharacter();

	ProtectEnemy* GetProtectEnemy()const{ return protectEnemy_; }
private:

	//敵管理クラス
	EnemyManager* enemyManager_ = nullptr;

	//プレイヤーのポインタ
	Player* playerPtr_ = nullptr;

	//スポーン用タイマー
	uint16_t spawnTimer_;

	//拠点を守る敵
	ProtectEnemy* protectEnemy_ = nullptr;
};

