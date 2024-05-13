#pragma once
#include "Bullet.h"

class Player;

class EnemyBullet :
    public Bullet{
    Vector3 velocity_;

    Player* player_ = nullptr;

    //寿命
    static const int32_t kLifeTime = 60 * 5;

    //デスタイマー
    int32_t deathTimer_ = kLifeTime;
    //ですフラグ
    bool isDead_ = false;

    bool isHoming_ = true;

#pragma region ホーミング用変数
    float speed_;

#pragma endregion

public:
    EnemyBullet();
    ~EnemyBullet();

    void Init(Model* model,const Vector3& position,const Vector3& vel);
    void HomingInit(float speed);
    void Update()override;
    void Homing();
    void Draw(const ViewProjection& viewProjection)override;

    void SetPlayer(Player* player){ player_ = player; }

    void OnCollision()override;

    bool GetIsHoming()const{ return isHoming_; };
};

