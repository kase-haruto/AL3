#pragma once

#include"Model.h"

class StrongholdManager{
public:
	
	/// <summary>
	/// コンストラクタ/デストラクタ
	/// </summary>
	StrongholdManager() = default;
	~StrongholdManager() = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

private:

};

