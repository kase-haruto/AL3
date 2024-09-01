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
#include"Enemy.h"
#include"TitleButtonManager.h"
#include"ProtectEnemy.h"


#include<stdint.h>
#include<memory>
#include<vector>

/// <summary>
/// ゲームシーン
/// </summary>
class ResultScene
	:public IScene{

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	ResultScene() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ResultScene()override = default;

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

	/// <summary>
	/// シーン遷移
	/// </summary>
	void TransitionScene();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// 3dモデル
	/// </summary>
	std::vector<std::unique_ptr<Model>> playerModels_;
	std::vector < std::unique_ptr<Model>>modelEnemy_;
	std::unique_ptr<Model> modelGround_ = nullptr;
	std::unique_ptr<Model> modelSkydome_ = nullptr;
	std::unique_ptr<Model> modelFlag_ = nullptr;

	//スプライト
	std::unique_ptr<Sprite>failureSprite_ = nullptr;
	std::unique_ptr<Sprite>successSprite_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>	
	XINPUT_STATE padState = {};
	XINPUT_STATE prePadState = {};
	
	ViewProjection viewProjection_;

	//地面
	std::unique_ptr<Ground> ground_ = nullptr;

	//天球
	std::unique_ptr<Skydome> skydome_ = nullptr;

	//プレイヤー
	std::unique_ptr<Player> player_ = nullptr;

	//敵
	std::unique_ptr<ProtectEnemy> protectEnemy_ = nullptr;

	//武器
	std::unique_ptr<WeaponManager>weaponManager_ = nullptr;

	WorldTransform flagTransform_;

	//サウンドデータ
	uint32_t resultSoundHandle_ = 0;
	uint32_t resultVoiceHandle_ = 0;

	/// <summary>
	/// カメラ
	/// </summary>
	std::unique_ptr<DebugCamera> debugCamera_;
	bool isDebugCameraActive_ = false;

};
