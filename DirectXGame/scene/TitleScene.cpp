#include "TitleScene.h"
#include "TextureManager.h"
#include"Hammer.h"

#include "SceneManager.h"
#include"GameScene.h"

#include <cassert>

TitleScene::TitleScene(){}


void TitleScene::Initialize(){

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	padState = {};
	prePadState = {};

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
	//ゲームスタートボタン
	buttonManager_ = std::make_unique<TitleButtonManager>();
	buttonManager_->Initialize();


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

	//=======================================================
	//		武器
	weaponManager_ = std::make_unique<WeaponManager>();
	weaponManager_->Initialize();

	//プレイヤーに初期武器をセット
	player_->SetWeapon(weaponManager_->GetWeapon(WeaponName::hammer));

	//=======================================================
	//		タイトルスプライトの初期化
	int32_t titleHandle = TextureManager::Load("./Resources/titleTexture.png");
	titleSprite_.reset(Sprite::Create(titleHandle, {100,40}));

	//=======================================================
	//		音声の初期化
	 //サウンドデータの読み込み
	selectSoundHandle_ = Audio::GetInstance()->LoadWave("isSelect.mp3");

	titleSoundHandle_ = Audio::GetInstance()->LoadWave("titleBGM.mp3");
	titleVoiceHandle_ = Audio::GetInstance()->PlayWave(titleSoundHandle_, true);
	Audio::GetInstance()->SetVolume(titleVoiceHandle_, 0.05f);
}

void TitleScene::Update(){


	//プレイヤーの更新
	player_->Update();

	//タイトルでのプレイヤーの向きは固定
	player_->SetRotationY(3.2f);

	//カメラの更新
	viewProjection_.TransferMatrix();

	//ボタンの更新
	buttonManager_->Update();

	//シーン遷移
	TransitionScene();
}

void TitleScene::Draw(){

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


	//プレイヤーの描画
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

	buttonManager_->Draw();
	titleSprite_->Draw();


	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void TitleScene::Finalize(){
	player_.reset();
	skydome_.reset();
	ground_.reset();
	buttonManager_.reset();
}

void TitleScene::TransitionScene(){
	prePadState = padState;
	// 現在の状態を取得
	XInputGetState(0, &padState);

	WORD currentButtonState = padState.Gamepad.wButtons;
	WORD preButtonState = prePadState.Gamepad.wButtons;

	// 'A' ボタンの状態を確認
	bool isXButtonPressed = (currentButtonState & XINPUT_GAMEPAD_A) != 0;
	bool wasXButtonPressed = (preButtonState & XINPUT_GAMEPAD_A) != 0;

	// 'A' ボタンが今押されたかどうかを確認
	bool isXButtonJustPressed = isXButtonPressed && !wasXButtonPressed;

	// シーン開始後の最初のフレームではボタン入力を無視
	static bool firstFrame = true;
	if (firstFrame){
		firstFrame = false;
		return;
	}

	if (isXButtonJustPressed){
		selectVoiceHandle_ = Audio::GetInstance()->PlayWave(selectSoundHandle_, false);
		Audio::GetInstance()->StopWave(titleVoiceHandle_);
		switch (buttonManager_->GetCurrentSelectButton()){
			case Play:
				SceneManager::GetInstance()->ChangeScene(std::make_unique<GameScene>());
				return;
			case Exit:
				SceneManager::GetInstance()->SetIsExit();
				return;
		}
	}
}

