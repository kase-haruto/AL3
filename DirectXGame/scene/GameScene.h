#pragma once

#include"Audio.h"
#include"DirectXCommon.h"
#include"Input.h"
#include"Model.h"
#include"Sprite.h"
#include"ViewProjection.h"
#include"WorldTransform.h"
#include"Player.h"
#include"Skydome.h"
#include"Ground.h"
#include"DebugCamera.h"
#include"FollowCamera.h"
#include"LockOn.h"
#include"CollisionManager.h"
#include"WeaponManager.h"
#include"EnemyStronghold.h"
#include"EnemyManager.h"
#include"IScene.h"
#include"PlayableCharacterManager.h"


#include<stdint.h>
#include<memory>
#include<vector>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene 
	:public IScene{

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene()override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update()override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize()override;

private:
	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollision();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	
	/// <summary>
	/// 3dモデル
	/// </summary>
	std::vector<std::unique_ptr<Model>> playerModels_;
	std::vector<std::unique_ptr<Model>> enemyModels_;
	std::unique_ptr<Model> moedlSkydome_ = nullptr;
	std::unique_ptr<Model> modelGround_ = nullptr;
	std::unique_ptr<WeaponManager>weaponManager_ = nullptr;
	std::unique_ptr<Model>modelStrongHold_ = nullptr;


	/// <summary>
	/// ゲームシーン用
	/// </summary>	
	ViewProjection viewProjection_;
	std::unique_ptr<Player>player_ = nullptr;
	std::unique_ptr<Player>protectPlayer_ = nullptr;
	std::unique_ptr<EnemyManager> enemyManager_ = nullptr;
	std::unique_ptr<Skydome>skydome_ = nullptr;
	std::unique_ptr<Ground>ground_ = nullptr;

	//敵の拠点
	std::vector<std::shared_ptr<EnemyStronghold>>enemyStronghold_;

	//衝突判定管理クラス
	std::unique_ptr<CollisionManager>collisionManager_ = nullptr;

	//操作キャラーの管理クラス
	std::unique_ptr<PlayableCharacterManager> characterManager_ = nullptr;
	

	/// <summary>
	/// カメラ
	/// </summary>
	std::unique_ptr<DebugCamera> debugCamera_;
	std::unique_ptr<FollowCamera> followCamera_;
	std::unique_ptr<LockOn> lockOn_;
	bool isDebugCameraActive_ = false;
	 
};
