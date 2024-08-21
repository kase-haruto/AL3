#pragma once

#include"Audio.h"
#include"DirectXCommon.h"
#include"Input.h"
#include"Model.h"
#include"Sprite.h"
#include"ViewProjection.h"
#include"IScene.h"
#include"DebugCamera.h"

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


	/// <summary>
	/// ゲームシーン用
	/// </summary>	



	/// <summary>
	/// カメラ
	/// </summary>
	std::unique_ptr<DebugCamera> debugCamera_;
	bool isDebugCameraActive_ = false;

};
