#pragma once
#include "IScene.h"
#include <memory>

/// <summary>
/// シーン管理クラス
/// </summary>
class SceneManager{
public:
    // シングルトンのインスタンスを取得するメソッド
    static SceneManager* GetInstance();

    // コピーコンストラクタと代入演算子を削除して、複製を防止
    SceneManager(const SceneManager&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;

    /// <summary>
    /// シーン遷移
    /// </summary>
    /// <param name="newScene"></param>
    void ChangeScene(std::unique_ptr<IScene> newScene);

    /// <summary>
    /// 更新
    /// </summary>
    void Update();

    /// <summary>
    /// 描画
    /// </summary>
    void Draw();

    /// <summary>
    /// ゲーム終了フラグをtrue
    /// </summary>
    void SetIsExit();

    /// <summary>
    /// ゲーム終了か
    /// </summary>
    /// <returns></returns>
    bool CheckIsExit();

private:
    // プライベートコンストラクタで外部からのインスタンス化を禁止
    SceneManager();
    ~SceneManager() = default;

    // 現在のシーン
    std::unique_ptr<IScene> currentScene_;

    // ゲーム終了
    bool isExitGame_ = false;

    // シーンが切り替わったかどうかを示すフラグ
    bool sceneChanged_ = false;

public:
    static bool isClear_;
    static bool isFailure_;//失敗
};
