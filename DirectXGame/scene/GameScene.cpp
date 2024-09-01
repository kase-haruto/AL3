#include "GameScene.h"
#include "TextureManager.h"
#include"Hammer.h"
#include"CameraManager.h"
#include"CollisionManager.h"
#include"SceneManager.h"
#include"ResultScene.h"
#include <cassert>

GameScene::GameScene():flagTaken_(3, false){}

GameScene::~GameScene(){}
#pragma optimize("", off) // 最適化を抑制
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
	ground_->Initialize(skydome_->GetScale() * 1.3f);


	///=====================================================
	//		プレイヤー
	playerModels_.push_back(std::unique_ptr<Model>(Model::CreateFromOBJ("playerBody", true)));
	playerModels_.push_back(std::unique_ptr<Model>(Model::CreateFromOBJ("playerHead", true)));
	playerModels_.push_back(std::unique_ptr<Model>(Model::CreateFromOBJ("player_L_arm", true)));
	playerModels_.push_back(std::unique_ptr<Model>(Model::CreateFromOBJ("player_R_arm", true)));
	player_ = std::make_unique<AttackPlayer>();
	std::vector<Model*> modelPointers;

	for (const auto& model : playerModels_){
		modelPointers.push_back(model.get());
	}

	// プレイヤークラスを初期化
	player_->Initialize(modelPointers);
	player_->SetPos({0.0f,0.0f,50.0f});
	player_->SetIsActive(true);

	// 拠点を守るキャラクターの初期化
	protectPlayer_ = std::make_unique<ProtectPlayer>();
	protectPlayer_->Initialize(modelPointers);
	protectPlayer_->SetPos({10.0f, 0.0f, 50.0f});
	protectPlayer_->SetMaxLife(120);
	protectPlayer_->UpdateMatrix();

	// 操作キャラの管理クラスの初期化
	characterManager_ = std::make_unique<PlayableCharacterManager>();

	// 操作キャラの追加
	characterManager_->AddCharacters(player_.get());
	characterManager_->AddCharacters(protectPlayer_.get());

	///=====================================================
	//		プレイヤー拠点
	modelStrongHold_.reset(Model::CreateFromOBJ("flag", true));
	modelPlayerStrongholdShield_.reset(Model::CreateFromOBJ("playerFlagDome", true));
	playerStronghold_ = std::make_unique<PlayerStronghold>();
	playerStronghold_->Initialize(modelStrongHold_.get());
	playerStronghold_->SetTranslation({0.0f,0.0f,40.0f});
	playerStronghold_->SetProtectPlayer(protectPlayer_.get());
	playerStronghold_->SetRangeModel(modelPlayerStrongholdShield_.get());

	///=====================================================
	//		敵拠点
	const int kNumEnemyStronghold = 3;
	//拠点のシールドモデル
	modelEnemyStrongholdShield_.reset(Model::CreateFromOBJ("enemyFlagDome", true));
	for (size_t i = 0; i < kNumEnemyStronghold; i++){
		auto stronghold = std::make_shared<EnemyStronghold>();
		stronghold->Initialize(modelStrongHold_.get());

		// 各拠点のx座標はそのまま、z座標を設定
		float zPosition = 150.0f + (i * 50.0f);
		if (i == 1){ // 中央の拠点
			zPosition += 80.0f;
		}

		stronghold->SetTranslation({-50 + (i * 50.0f), 1.0f, zPosition});
		stronghold->SetPlayerPtr(player_);
		stronghold->SetRangeModel(modelEnemyStrongholdShield_.get());
		stronghold->UpdateMatrix();

		enemyStronghold_.push_back(stronghold);
	}

	///=====================================================
	//		敵
	enemyManager_ = std::make_unique<EnemyManager>();
	enemyManager_->SetStronghold(enemyStronghold_);
	enemyManager_->Initialize();

	enemyStation_ = std::make_unique<EnemyStation>();
	enemyStation2_ = std::make_unique<EnemyStation>();
	enemyStation_->Initialize();
	enemyStation2_->Initialize();
	Vector3 offset = {60.0f,0.0f,0.0f};

	// ステーション1の配置
	enemyStation_->SetPosition(enemyStronghold_[1]->GetWorldPosition() + offset);
	enemyStation_->SetPlayerStrongholdPtr(playerStronghold_.get());
	enemyStation_->SetEnemyManager(enemyManager_.get());

	// ステーション2の配置 (反対側に配置)
	enemyStation2_->SetPosition(enemyStronghold_[1]->GetWorldPosition() - offset);
	enemyStation2_->SetPlayerStrongholdPtr(playerStronghold_.get());
	enemyStation2_->SetEnemyManager(enemyManager_.get());


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
	CameraManager::GetInstance()->SetActiveCamera(followCamera_.get());

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
	CollisionManager::GetInstance()->Initialize();

	//=======================================================
	//		クリア条件の旗のスプライト
	Vector2 flagSpritePos {WinApp::kWindowWidth * 0.5f - 100.0f , WinApp::kWindowHeight - 80.0f};
	int flagTexture = TextureManager::Load("./Resources/flagTexture.png");
	for (size_t i = 0; i < enemyStronghold_.size(); i++){
		auto flag = std::make_unique<Sprite>();
		flag.reset(Sprite::Create(flagTexture, flagSpritePos, {1, 1, 1, 1}, {0.5f,0.5f}));
		flag->SetSize({100.0f,100.0f});
		flagSpritePos.x += 100;
		flagSprite_.push_back(std::move(flag));
	}
}

void GameScene::Update(){
	prePadState = padState;
	// 現在の状態を取得
	XInputGetState(0, &padState);

	WORD currentButtonState = padState.Gamepad.wButtons;
	WORD preButtonState = prePadState.Gamepad.wButtons;

	// 'X' ボタンの状態を確認
	bool isXButtonPressed = (currentButtonState & XINPUT_GAMEPAD_B) != 0;
	bool wasXButtonPressed = (preButtonState & XINPUT_GAMEPAD_B) != 0;

	// 'X' ボタンが今押されたかどうかを確認
	bool isXButtonJustPressed = isXButtonPressed && !wasXButtonPressed;

	if (isXButtonJustPressed){
		// キャラクターを切り替える
		characterManager_->SwitchCharacter((characterManager_->GetCurrentCharacterIndex() + 1) % 2);
	}


	// プレイヤーの更新
	player_->Update();
	protectPlayer_->Update();

	playerStronghold_->Update();

	// 敵の更新
	enemyManager_->Update();

	// 敵拠点の更新
	for (size_t i = 0; i < 3; i++){
		enemyStronghold_[i]->Update();

		// フラッグがまだ取られていない場合
		if (!flagTaken_[i] && enemyStronghold_[i]->GetIsTaked()){
			flagCount_++;
			flagTaken_[i] = true;  // フラッグが取られたことを記録
		}
	}

	enemyStation_->Update();
	enemyStation2_->Update();

	// 総当たりでオブジェクトの衝突判定
	CheckAllCollision();



#ifdef _DEBUG 
	// 判定の可視化のtransformの更新
	CollisionManager::GetInstance()->UpdateWorldTransform();
#endif // _DEBUG

	// ロックオン機能の更新
	lockOn_->Update(enemyManager_->GetAllEnemies(), viewProjection_);

	// デバッグ用のカメラ
	CameraManager* camManager = CameraManager::GetInstance();
	camManager->GetFollowCamera()->Update();
	if (isDebugCameraActive_){
		// デバッグ用のカメラ
		debugCamera_->Update();
		// 情報の受け渡し
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
	} else{
		// 情報の受け渡し
		viewProjection_.matView = camManager->GetFollowCamera()->GetViewProjection().matView;
		viewProjection_.matProjection = camManager->GetFollowCamera()->GetViewProjection().matProjection;
		// 更新と転送
	}
	viewProjection_.TransferMatrix();

	// カメラの切り替え
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_RETURN)){
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif // _DEBUG

	if (flagCount_ >= enemyStronghold_.size() || playerStronghold_->GetIsTaked()){
		SceneManager::GetInstance()->ChangeScene(std::make_unique<ResultScene>());
	}
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
	CollisionManager::GetInstance()->Draw(viewProjection_);
#endif // _DEBUG


	//=========================================================
	//	プレイヤーの描画
	//=========================================================
	player_->Draw(viewProjection_);
	protectPlayer_->Draw(viewProjection_);
	playerStronghold_->Draw(viewProjection_);
	//=========================================================
	//	敵の描画
	//=========================================================
	enemyManager_->Draw(viewProjection_);
	enemyStation_->Draw(viewProjection_);
	enemyStation2_->Draw(viewProjection_);

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

	for (size_t i = 0; i < 3; i++){
		if (flagTaken_[i]){
			flagSprite_[i]->Draw();
		}
	}

	//ロックオンスプライト描画
	lockOn_->Draw();

	player_->DrawUiSprite();
	protectPlayer_->DrawUiSprite();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}


void GameScene::CheckAllCollision(){
	auto collisionManager = CollisionManager::GetInstance();
	//衝突マネージャーのリセット
	collisionManager->Reset();

	//=============================================================
	//		コライダーをリストに登録
	//=============================================================

	//プレイヤー
	collisionManager->AddCollider(player_.get());
	collisionManager->AddCollider(protectPlayer_.get());

	//武器
	collisionManager->AddCollider(weaponManager_->GetWeapon(WeaponName::hammer));
	collisionManager->AddCollider(weaponManager_->GetWeapon(WeaponName::gun));

	//敵すべてについて
	for (const auto& enemy : enemyManager_->GetAllEnemies()){
		collisionManager->AddCollider(enemy.get());
	}


	//=============================================================
	//		衝突判定と応答
	//=============================================================

	collisionManager->CheckAllCollidion();

	//playerと敵の拠点の判定と応答
	for (const auto& enemyStronghold : enemyStronghold_){
		collisionManager->Player2Stronghold(player_.get(), enemyStronghold.get());
	}

	for (const auto& enemy : enemyManager_->GetAllEnemies()){
		collisionManager->Enemy2Stronghold(enemy.get(), playerStronghold_.get());
	}
}

void GameScene::Finalize(){
	for (size_t i = 0; i < 3; i++){
		flagTaken_[i] = false;
	}
}
#pragma optimize("", on) // 最適化を抑制