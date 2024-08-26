#pragma once

#include"Button.h"

#include<vector>

enum SelectMode{
	GamePlay,
	Setting,
	Exit,
};

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

private:
	//ボタン
	std::vector<std::unique_ptr<Button>> buttons_;

	//選択されている
	int currentSelect_ = GamePlay;
};

