#pragma once

#include"Button.h"

#include<vector>
#include<Xinput.h>

namespace {
	enum ButtonMode{
		Play,
		Exit,
	};
}

class TitleButtonManager{
public:
	TitleButtonManager() = default;
	~TitleButtonManager() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	int GetCurrentSelectButton()const{ return currentSelectButton_; }

private:
	//ボタン
	std::vector<std::unique_ptr<Button>> buttons_;

	//現在選択されているボタン
	int currentSelectButton_ = Play;

	XINPUT_STATE padState = {};
	XINPUT_STATE prePadState = {};

	//サウンドデータ
	uint32_t selectSoundHandle_ = 0;
	uint32_t selectVoiceHandle_ = 0;

};

