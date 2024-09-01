#include "TitleButtonManager.h"
#include"SceneManager.h"
#include"GameScene.h"
#include"MyFunc.h"
#include"Input.h"

void TitleButtonManager::Initialize(){

	Vector3 buttonPos = {-100.0f,400.0f,0.0f};

	for (size_t i = 0; i < 2; i++){
		auto button = std::make_unique<Button>();
		const std::string filePath = (i == 0) ? "./Resources/startButton.png" : "./Resources/exitButton.png";
		button->Initialize(buttonPos, {50.0f,50.0f}, filePath);
		buttonPos.y += 150.0f;
		buttons_.push_back(std::move(button));
	}

	currentSelectButton_ = Play;
	//=======================================================
	//		音声の初期化
	 //サウンドデータの読み込み
	selectSoundHandle_ = Audio::GetInstance()->LoadWave("moveButton.mp3");
}

void TitleButtonManager::Update(){


	////////////////////////////////////////////////////////
	//      ボタンを画面外から動かしてくる
	////////////////////////////////////////////////////////
	static int frameCount = 0; // フレームカウントを保持

	for (int i = 0; i < buttons_.size(); i++){
		int startFrame = i * 10; // 各ボタンの移動開始タイミングをずらす（10フレーム毎）

		if (frameCount >= startFrame){ // 指定のフレーム数に達したら移動を開始
			Vector2 buttonTargetPos = {330.0f, 400.0f + i * 150.0f};
			Vector2 buttonPos = Lerp(buttons_[i]->GetPos(), buttonTargetPos, 0.1f);
			buttons_[i]->SetPos(buttonPos);
		}
	}

	//目標座標に達するまでインクリメント
	if (buttons_[1]->GetPos().x<= 200.0f){
		frameCount++; // フレームをインクリメント
	}

	////////////////////////////////////////////////////////
	//      ボタンの更新
	////////////////////////////////////////////////////////
	prePadState = padState;
	// 現在の状態を取得
	XInputGetState(0, &padState);

	WORD currentButtonState = padState.Gamepad.wButtons;
	WORD preButtonState = prePadState.Gamepad.wButtons;

	// '上' ボタンの状態を確認
	bool isUpButtonPressed = (currentButtonState & XINPUT_GAMEPAD_DPAD_UP) != 0;
	bool wasUpButtonPressed = (preButtonState & XINPUT_GAMEPAD_DPAD_UP) != 0;

	// '上' ボタンが今押されたかどうかを確認
	bool isUpButtonJustPressed = isUpButtonPressed && !wasUpButtonPressed;

	// '下' ボタンの状態を確認
	bool isDownButtonPressed = (currentButtonState & XINPUT_GAMEPAD_DPAD_DOWN) != 0;
	bool wasDownButtonPressed = (preButtonState & XINPUT_GAMEPAD_DPAD_DOWN) != 0;

	// '下' ボタンが今押されたかどうかを確認
	bool isDownButtonJustPressed = isDownButtonPressed && !wasDownButtonPressed;


	// 下キーが押された場合
	if (isDownButtonJustPressed){
		selectVoiceHandle_ = Audio::GetInstance()->PlayWave(selectSoundHandle_, false);
		if (currentSelectButton_ < buttons_.size() - 1){ // 範囲内でインクリメント
			currentSelectButton_++;
		}
	}
	// 上キーが押された場合
	else if (isUpButtonJustPressed){
		selectVoiceHandle_ = Audio::GetInstance()->PlayWave(selectSoundHandle_, false);
		if (currentSelectButton_ > 0){ // 範囲内でデクリメント
			currentSelectButton_--;
		}
	}

	// 現在選択されているボタンを更新
	for (size_t i = 0; i < buttons_.size(); i++){
		if (i == currentSelectButton_){
			buttons_[i]->SetIsSelected(true);
		} else{
			buttons_[i]->SetIsSelected(false);
		}
		buttons_[i]->Update();
	}

}

void TitleButtonManager::Draw(){
	for (size_t i = 0; i < buttons_.size(); i++){
		buttons_[i]->Draw();
	}
}
