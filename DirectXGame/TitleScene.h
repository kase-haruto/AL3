#pragma once

#include"Audio.h"
#include"DirectXCommon.h"
#include"Input.h"
#include"Model.h"
#include"Sprite.h"
#include"ViewProjection.h"
#include"IScene.h"
#include"DebugCamera.h"
#include"Button.h"
#include"Player.h"
#include"WeaponManager.h"
#include"Ground.h"
#include"Skydome.h"
#include"TransitionSelectCube.h"


#include<stdint.h>
#include<memory>
#include<vector>

/// <summary>
/// ゲームシーン
/// </summary>
class TitleScene
	:public IScene{

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	TitleScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TitleScene()override = default;

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
	/// 解放処理
	/// </summary>
	void Finalize()override;

private:

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// 3dモデル
	/// </summary>
	std::vector<std::unique_ptr<Model>> playerModels_;
	std::unique_ptr<Model> modelGround_ = nullptr;
	std::unique_ptr<Model> modelSkydome_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>	
	ViewProjection viewProjection_;

	//地面
	std::unique_ptr<Ground> ground_ = nullptr;

	//天球
	std::unique_ptr<Skydome> skydome_ = nullptr;

	//キューブ
	std::unique_ptr<TransitionSelectCube> cube_ = nullptr;

	//ゲームスタートボタン
	std::unique_ptr<Button> gameStartButton_ = nullptr;

	//ゲーム終了ボタン
	std::unique_ptr<Button> exitButton_ = nullptr;

	//プレイヤー
	std::unique_ptr<Player> player_ = nullptr;

	//武器
	std::unique_ptr<WeaponManager>weaponManager_ = nullptr;

	/// <summary>
	/// カメラ
	/// </summary>
	std::unique_ptr<DebugCamera> debugCamera_;
	bool isDebugCameraActive_ = false;

};
