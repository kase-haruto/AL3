#pragma once
#include "Actor.h"
#include"EnemyBullet.h"
#include<memory>
//行動フェーズ
enum class Phase{
    Approach,//接近する
    Leave,//離脱する
};


class Enemy :
    public Actor{
    Vector3 velocity_;
    bool isMove = false;

    uint32_t cooltimer = 0;
    const uint32_t kShootInterval = 60;

    std::list<std::unique_ptr<EnemyBullet>> bullets_;

    //フェーズ
    Phase phase_ = Phase::Approach;

private://メンバ関数

    /// <summary>
    /// 接近フェーズの初期化
    /// </summary>
    void ApproachInitialize();

   
    /// <summary>
    /// 接近フェーズ
    /// </summary>
    void ApproachPhase();

    /// <summary>
    /// 離脱フェーズ
    /// </summary>
    void LeavePhase();
    
    /// <summary>
    /// 行動フェーズの切り替え
    /// </summary>
    void SwitchPhase();

    /// <summary>
    /// 弾を撃つ
    /// </summary>
    void Shoot();

public:

    Enemy();
    ~Enemy();

    void Init(Model* model);
    void Update()override;
    void Draw(ViewProjection& viewprojection)override;

    void SetIsMove(const bool isMove_){ isMove = isMove_; }

};

