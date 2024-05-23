#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include"AxisIndicator.h"
#include<cmath>
#include"CollisionManager.h"
#include<imgui.h>

GameScene::GameScene(){}

GameScene::~GameScene(){ delete model_, modelSkydome_; }

void GameScene::Initialize(){

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	//軸方向表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するプロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	// デバッグ用のカメラ
	debugCamera_ = std::make_unique<DebugCamera>(1280, 720);
	
	viewProjection_.Initialize();

	//レールカメラ
	railCamera_ = std::make_unique<RailCamera>();
	railCamera_->Initialize(viewProjection_.GetWorldPosition());

	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	skydome_ = std::make_unique<Skydome>(modelSkydome_);
	skydome_->Initialize();

	model_ = Model::Create();

	player_ = std::make_unique<Player>();
	Vector3 playerPos = {0.0f,0.0f,30.0f};
	player_->Init(model_,playerPos);
	player_->SetParent(&railCamera_->GetWorldTransform());

	enemy_ = std::make_unique<Enemy>();
	enemy_->SetPlayer(player_.get());
	enemy_->Init(model_);

	
	
	
}

void GameScene::Update(){
	//=========================================================
	//	 playerの更新処理
	//=========================================================
	player_->Update();


	//=========================================================
	//	 enemyの更新処理
	//=========================================================
	if (enemy_){
		if (input_->TriggerKey(DIK_SPACE)){
			enemy_->SetIsMove(true);
		}
		enemy_->Update();

	}
	

	//=========================================================
	//	 衝突判定
	//=========================================================
	CollisionManager::GetInstance()->Update();


	//=========================================================
	//	カメラの更新処理
	//=========================================================
	railCamera_->Update();

#ifdef _DEBUG

	if (isDebugCameraActive_){
		// デバッグ用のカメラ
		debugCamera_->Update();
		//情報の受け渡し
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		//転送
	} else{
		//情報の受け渡し
		viewProjection_.matView = railCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
		//更新と転送
	}
	viewProjection_.TransferMatrix();

#endif // _DEBUG

#ifdef _DEBUG

	//カメラの切り替え
	if (input_->TriggerKey(DIK_RETURN)){
		isDebugCameraActive_ = !isDebugCameraActive_;
	}

#endif // _DEBUG
}

void GameScene::Draw(){

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	//=========================================================
	//	天球の描画
	//=========================================================
	skydome_->Draw(viewProjection_);

	//=========================================================
	//	enemyの描画
	//=========================================================
	enemy_->Draw(viewProjection_);

	//=========================================================
	//	playerの描画
	//=========================================================
	player_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
