#pragma once

#include<memory>

class SceneManager;

/// <summary>
/// シーンの基底クラス
/// </summary>
class IScene{
public:

	/// <summary>
	/// コンストラクタ /
	/// デストラクタ
	/// </summary>
	IScene() = default;
	virtual ~IScene() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// 終了処理
	/// </summary>
	virtual void Finalize() = 0;

protected:
	//シーン管理クス
	SceneManager* sceneManager;
};

