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
#include"Enemy.h"

#include<stdint.h>
#include<memory>
#include<vector>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

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

	/// <summary>
	/// ゲームシーン用
	/// </summary>	
	ViewProjection viewProjection_;
	std::unique_ptr<Player>player_ = nullptr;
	std::list<std::unique_ptr<Enemy>>enemies_;
	//std::unique_ptr<Enemy>enemy_ = nullptr;
	std::unique_ptr<Skydome>skydome_ = nullptr;
	std::unique_ptr<Ground>ground_ = nullptr;

	/// <summary>
	/// カメラ
	/// </summary>
	std::unique_ptr<DebugCamera> debugCamera_;
	std::unique_ptr<FollowCamera> followCamera_;
	std::unique_ptr<LockOn> lockOn_;
	bool isDebugCameraActive_ = false;
	 
};
