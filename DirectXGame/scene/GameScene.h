#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include"DebugCamera.h"
#include"Player.h"
#include"Enemy.h"
#include<memory>
#include"Collider.h"
#include"Skydome.h"
#include"RailCamera.h"
#include"EnemyBullet.h"
#include<sstream>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene{

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 敵の球の設定
	/// </summary>
	void SetEnemyBullet(std::unique_ptr<EnemyBullet> enemyBullet);

private:
	/// <summary>
	/// 敵の球の更新
	/// </summary>
	void EnemyBulletUpdate();
	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void LoadEnemyPopData();
	/// <summary>
	/// 敵発生データの更新
	/// </summary>
	void UpdateEnemyPopData();
	/// <summary>
	/// 敵発生
	/// </summary>
	void PopEnemy(Vector3 pos);

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	std::unique_ptr<RailCamera> railCamera_ = nullptr;
	ViewProjection viewProjection_;
	std::unique_ptr<Player> player_ = nullptr;
	std::list<std::unique_ptr<Enemy>> enemies_;
	std::list < std::unique_ptr<EnemyBullet>>enemyBullet_;
	std::unique_ptr<Skydome>skydome_ = nullptr;
	Model* modelSkydome_ = nullptr;
	Model* model_ = nullptr;

	//===============
	//敵発生コマンド
	std::stringstream enemyPopCommands;


	/// <summary>
	/// デバッグ用
	/// </summary>
	bool isDebugCameraActive_ = false;
	std::unique_ptr<DebugCamera> debugCamera_ = nullptr;
};
