#include "GameScene.h"
#include "TextureManager.h"
#include"Hammer.h"
#include"CameraManager.h"

#include <cassert>

GameScene::GameScene(){}

GameScene::~GameScene(){}

void GameScene::Initialize(){

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	const int kWindowWidth = 1280;
	const int kWindowHeight = 720;

	//viewProjectionの初期化
	viewProjection_.Initialize();

	///=====================================================
	//		天球
	moedlSkydome_.reset(Model::CreateFromOBJ("skydome", true));
	skydome_ = std::make_unique<Skydome>(moedlSkydome_.get());
	skydome_->Initialize();


	///=====================================================
	//		地面
	modelGround_.reset(Model::CreateFromOBJ("ground", true));
	ground_ = std::make_unique<Ground>(modelGround_.get());
	ground_->Initialize(skydome_->GetScale());


	///=====================================================
	//		プレイヤー
	playerModels_.push_back(std::unique_ptr<Model>(Model::CreateFromOBJ("playerBody", true)));
	playerModels_.push_back(std::unique_ptr<Model>(Model::CreateFromOBJ("playerHead", true)));
	playerModels_.push_back(std::unique_ptr<Model>(Model::CreateFromOBJ("player_L_arm", true)));
	playerModels_.push_back(std::unique_ptr<Model>(Model::CreateFromOBJ("player_R_arm", true)));
	player_ = std::make_unique<Player>();
	std::vector<Model*> modelPointers;

	for (const auto& model : playerModels_){
		modelPointers.push_back(model.get());
	}

	// プレイヤークラスを初期化
	player_->Initialize(modelPointers);
	player_->SetIsActive(true);

	//拠点を守るキャラクター
	protectPlayer_ = std::make_unique<Player>();
	protectPlayer_->Initialize(modelPointers);
	protectPlayer_->SetPos({10.0f,0.0f,0.0f});

	//操作キャラの管理クラス
	characterManager_ = std::make_unique<PlayableCharacterManager>();
	//操作キャラの追加
	characterManager_->AddCharacters(player_.get());
	characterManager_->AddCharacters(protectPlayer_.get());


	///=====================================================
	//		敵拠点
	modelStrongHold_.reset(Model::CreateFromOBJ("cube", true));
	const int kNumEnemyStronghold = 3;
	for (size_t i = 0; i < kNumEnemyStronghold; i++){
		auto stronghold = std::make_shared<EnemyStronghold>();
		stronghold->Initialize(modelStrongHold_.get());

		// 各拠点のx座標はそのまま、z座標を設定
		float zPosition = 150.0f + (i * 50.0f);
		if (i == 1){ // 中央の拠点
			zPosition += 80.0f;
		}

		stronghold->SetTranslation({-50 + (i * 50.0f), 1.0f, zPosition});
		enemyStronghold_.push_back(stronghold);
	}

	///=====================================================
	//		敵
	enemyManager_ = std::make_unique<EnemyManager>();
	enemyManager_->SetStronghold(enemyStronghold_);
	enemyManager_->Initialize();


	//=======================================================
	//		武器
	weaponManager_ = std::make_unique<WeaponManager>();
	weaponManager_->Initialize();

	//プレイヤーに初期武器をセット
	player_->SetWeapon(weaponManager_->GetWeapon(WeaponName::hammer));
	protectPlayer_->SetWeapon(weaponManager_->GetWeapon(WeaponName::gun));


	///=====================================================
	//		追従カメラ
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
	followCamera_->SetTarget(&player_->GetWorldTransform());
	player_->SetViewProjection(&followCamera_->GetViewProjection());
	protectPlayer_->SetViewProjection(&followCamera_->GetViewProjection());

	//======================================================
	//		ロックオン
	lockOn_ = std::make_unique<LockOn>();
	lockOn_->Initialize();

	followCamera_->SetLockOn(lockOn_.get());
	player_->SetLockOn(lockOn_.get());

	///=====================================================
	//		デバッグカメラ
	debugCamera_ = std::make_unique<DebugCamera>(kWindowWidth, kWindowHeight);

	//=======================================================
	//		衝突判定
	collisionManager_ = std::make_unique<CollisionManager>();
	collisionManager_->Initialize();
}

void GameScene::Update(){
	//プレイヤーの更新
	player_->Update();

	protectPlayer_->Update();

	//操作キャラの管理
	characterManager_->Update();

	//敵の更新
	enemyManager_->Update();

	//敵拠点の更新
	for (size_t i = 0; i < 3; i++){
		enemyStronghold_[i]->Update();
	}

	//総当たりでオブジェクトの衝突判定
	CheckAllCollision();

#ifdef _DEBUG 
	//判定の可視化のtransformの更新
	collisionManager_->UpdateWorldTransform();
#endif // _DEBUG

	//ロックオン機能の更新
	lockOn_->Update(enemyManager_->GetAllEnemies(), viewProjection_);

#ifdef _DEBUG
	// デバッグ用のカメラ
	followCamera_->Update();
	if (isDebugCameraActive_){
		// デバッグ用のカメラ
		debugCamera_->Update();
		//情報の受け渡し
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
	} else{
		//情報の受け渡し
		viewProjection_.matView = followCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = followCamera_->GetViewProjection().matProjection;
		//更新と転送
	}
	viewProjection_.TransferMatrix();

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
	//	地面の描画
	//=========================================================
	ground_->Draw(viewProjection_);


#ifdef _DEBUG
	//衝突範囲の可視化
	collisionManager_->Draw(viewProjection_);
#endif // _DEBUG


	//=========================================================
	//	プレイヤーの描画
	//=========================================================
	player_->Draw(viewProjection_);
	protectPlayer_->Draw(viewProjection_);

	//=========================================================
	//	敵の描画
	//=========================================================
	enemyManager_->Draw(viewProjection_);

	//=========================================================
	//	敵の拠点の描画
	//=========================================================
	for (size_t i = 0; i < 3; i++){
		enemyStronghold_[i]->Draw(viewProjection_);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	
	//ロックオンスプライト描画
	lockOn_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}


void GameScene::CheckAllCollision(){
	//衝突マネージャーのリセット
	collisionManager_->Reset();

	//=============================================================
	//		コライダーをリストに登録
	//=============================================================

	//プレイヤー
	collisionManager_->AddCollider(player_.get());
	collisionManager_->AddCollider(protectPlayer_.get());

	//武器
	collisionManager_->AddCollider(weaponManager_->GetWeapon(WeaponName::hammer));
	collisionManager_->AddCollider(weaponManager_->GetWeapon(WeaponName::gun));

	//敵すべてについて
	for (const auto& enemy:enemyManager_->GetAllEnemies()){
		collisionManager_->AddCollider(enemy.get());
	}


	//=============================================================
	//		衝突判定と応答
	//=============================================================
	
	collisionManager_->CheckAllCollidion();
}

void GameScene::Finalize(){

}