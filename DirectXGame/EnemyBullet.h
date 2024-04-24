#pragma once
#include "Bullet.h"

class EnemyBullet :
    public Bullet{
    Vector3 velocity_;


    //寿命
    static const int32_t kLifeTime = 60 * 5;

    //デスタイマー
    int32_t deathTimer_ = kLifeTime;
    //ですフラグ
    bool isDead_ = false;
public:
    EnemyBullet();
    ~EnemyBullet();

    void Init(Model* model,const Vector3& position,const Vector3& vel);
    void Update()override;
    void Draw(const ViewProjection& viewProjection)override;
};

