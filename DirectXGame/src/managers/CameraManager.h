#pragma once
#include "FollowCamera.h"

/// <summary>
/// カメラの管理
/// </summary>
class CameraManager{
public:

    /// <summary>
    /// インスタンスの取得
    /// </summary>
    /// <returns></returns>
    static CameraManager* GetInstance();

    /// <summary>
    /// 追従カメラの取得
    /// </summary>
    /// <returns></returns>
    FollowCamera* GetFollowCamera() const;

    /// <summary>
    /// カメラの切り替え
    /// </summary>
    /// <param name="camera"></param>
    void SetActiveCamera(FollowCamera* camera);

    /// <summary>
    /// カメラの更新
    /// </summary>
    void Update();

private:
    // コンストラクタをプライベートにしてシングルトンを強制
    CameraManager() = default;

    // コピーコンストラクタと代入演算子も削除
    CameraManager(const CameraManager&) = delete;
    CameraManager& operator=(const CameraManager&) = delete;

    //アクティブなカメラ
    FollowCamera* followCamera_ = nullptr;
};
