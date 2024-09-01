#pragma once

//拠点
#include"EnemyStronghold.h"

//敵
#include "Enemy.h"
#include"ProtectEnemy.h"
#include"AttackEnemy.h"

//カメラ
#include"ViewProjection.h"

//モデル
#include"Model.h"

#include<memory>
#include<list>
#include<vector>

class EnemyStronghold;
class Player;

/// <summary>
/// 敵管理クラス
/// </summary>
class EnemyManager{
public:

	/// <summary>
	/// コンストラクタ
	/// デストラクタ
	/// </summary>
	EnemyManager() = default;
	~EnemyManager() = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// すべての敵を取得
	/// </summary>
	/// <returns></returns>
	const std::list<std::unique_ptr<Enemy>>& GetAllEnemies()const;

	/// <summary>
	/// 敵拠点を取得
	/// </summary>
	/// <param name="stronghold"></param>
	void SetStronghold(const std::vector<std::shared_ptr<EnemyStronghold>>& stronghold);

	/// <summary>
	/// 敵を追加
	/// </summary>
	/// <param name="enemy"></param>
	void AddEnemy(std::unique_ptr<Enemy>&& enemy);


private:

	///===================================
	///	描画用モデル
	///===================================
	std::vector< std::unique_ptr<Model>>protectEnemyModels_;

	///===================================
	///	敵
	///===================================
	//拠点を守る敵
	std::vector<std::unique_ptr<ProtectEnemy>> protectEnemies_;

	//プレイヤーに追従する敵
	std::vector<std::unique_ptr<AttackEnemy>> attackEnemies_;

	//すべての敵のリスト
	std::list<std::unique_ptr<Enemy>> allEnemies_;

	///===================================
	///	敵拠点
	///===================================
	std::vector<std::shared_ptr<EnemyStronghold>> stronghold_;

	
};