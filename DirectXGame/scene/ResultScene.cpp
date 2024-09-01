#include "ResultScene.h"
#include "TextureManager.h"
#include"Hammer.h"

#include "SceneManager.h"
#include"GameScene.h"
#include"TitleScene.h"
#include <cassert>

#include"MyFunc.h"

void ResultScene::Initialize(){

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();


	viewProjection_.Initialize();

	///=====================================================
	//		天球
	modelSkydome_.reset(Model::CreateFromOBJ("skydome", true));
	skydome_ = std::make_unique<Skydome>(modelSkydome_.get());
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
	player_ = std::make_unique<Player>();
	std::vector<Model*> modelPointers;

	for (const auto& model : playerModels_){
		modelPointers.push_back(model.get());
	}

	// プレイヤークラスを初期化
	player_->Initialize(modelPointers);
	player_->SetPos({3.0f,0.0f,-37.0f});
	player_->SetIsActive(true);


	modelEnemy_.push_back(std::unique_ptr<Model>(Model::CreateFromOBJ("enemy", true)));
	modelEnemy_.push_back(std::unique_ptr<Model>(Model::CreateFromOBJ("enemy_arm", true)));
	protectEnemy_ = std::make_unique<ProtectEnemy>();
	std::vector<Model*>modelPtr;
	for (const auto& model : modelEnemy_){
		modelPtr.push_back(model.get());
	}
	protectEnemy_->Initialize(modelPtr);
	protectEnemy_->SetPos({5.0f,0.0f,-28.0f});

	modelFlag_.reset(Model::CreateFromOBJ("flag", true));
	flagTransform_.Initialize();
	flagTransform_.translation_ = {-1.0f,0.0f,-30.0f};
	flagTransform_.UpdateMatrix();
	

	//=======================================================
	//		武器
	weaponManager_ = std::make_unique<WeaponManager>();
	weaponManager_->Initialize();

	//プレイヤーに初期武器をセット
	player_->SetWeapon(weaponManager_->GetWeapon(WeaponName::hammer));

	//=======================================================
	//		sprite
	int failureHandle = TextureManager::Load("failure.png");
	int successHandle = TextureManager::Load("success.png");

	failureSprite_.reset(Sprite::Create(failureHandle, {0,-100}));
	successSprite_.reset(Sprite::Create(successHandle, {1000,-100}));

	//=======================================================
	//		音源
	 //サウンドデータの読み込み
	resultSoundHandle_ = Audio::GetInstance()->LoadWave("resultBGM.mp3");
	resultVoiceHandle_ = Audio::GetInstance()->PlayWave(resultSoundHandle_, true);
	Audio::GetInstance()->SetVolume(resultVoiceHandle_, 0.1f);
}

void ResultScene::Update(){
	// 現在のスプライトの位置を取得
	Vector2 currentFailurePosition = failureSprite_->GetPosition();
	Vector2 failurePos {300,100};
	Vector2 newFailurePosition;
	newFailurePosition.x = Lerp(currentFailurePosition.x, failurePos.x, 0.1f);
	newFailurePosition.y = Lerp(currentFailurePosition.y, failurePos.y, 0.1f);
	// 新しい位置にスプライトを移動
	failureSprite_->SetPosition(newFailurePosition);

	// 現在のスプライトの位置を取得
	Vector2 currentSuccessPosition = successSprite_->GetPosition();
	Vector2 SuccessPos {300,100};
	Vector2 newSuccessPosition;
	newSuccessPosition.x = Lerp(currentSuccessPosition.x, failurePos.x, 0.1f);
	newSuccessPosition.y = Lerp(currentSuccessPosition.y, failurePos.y, 0.1f);
	// 新しい位置にスプライトを移動
	successSprite_->SetPosition(newSuccessPosition);

	//プレイヤーの更新
	player_->Update();

	protectEnemy_->UpdateResultScene();


	//タイトルでのプレイヤーの向きは固定
	player_->SetRotationY(3.2f);

	//カメラの更新
	viewProjection_.TransferMatrix();

	//シーン遷移
	TransitionScene();
}

void ResultScene::Draw(){

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

	modelFlag_->Draw(flagTransform_,viewProjection_);

	if (SceneManager::isClear_){
		//プレイヤーの描画
		player_->Draw(viewProjection_);
	} 
	
	if (SceneManager::isFailure_){
		protectEnemy_->Draw(viewProjection_);
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
	

	if (SceneManager::isFailure_){
		failureSprite_->Draw();
	}

	if (SceneManager::isClear_){
		successSprite_->Draw();
	}


	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void ResultScene::Finalize(){
	player_.reset();
	skydome_.reset();
	ground_.reset();

	SceneManager::isClear_ = false;
	SceneManager::isFailure_ = false;
}

void ResultScene::TransitionScene(){
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
		// ボタンの状態をリセット
		prePadState = {};
		padState = {};
		Audio::GetInstance()->StopWave(resultVoiceHandle_);
		SceneManager::GetInstance()->ChangeScene(std::make_unique<TitleScene>());
	}
}

