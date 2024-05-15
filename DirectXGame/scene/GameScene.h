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

private:
	/// <summary>
	/// コライダー2つの衝突判定と応答
	/// </summary>
	/// <param name="colliderA"></param>
	/// <param name="colliderB"></param>
	void CheckCollisionPair(Collider* colliderA,Collider* colliderB);

	void CheckAllCollisions();

	float CheckDistance(Vector3 v1,Vector3 v2);
	bool IsOnCollision(float distance,float radius1,float radius2);

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	ViewProjection viewProjection_;
	std::unique_ptr<Player> player_;
	std::unique_ptr<Enemy> enemy_;
	Model* model_;

	/// <summary>
	/// デバッグ用
	/// </summary>
	bool isDebugCameraActive_ = false;
	DebugCamera* debugCamera_ = nullptr;
};
